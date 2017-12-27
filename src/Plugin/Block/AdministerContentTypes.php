<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'Administer Content Types'.
 *
 * @Block(
 * id = "administer_content_types",
 * admin_label = @Translation("Administer Content Types"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerContentTypes extends BlockBase implements BlockPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $types = node_type_get_types();
    $access = \Drupal::currentUser()->hasPermission('administer content types');
    $config = $this->getConfiguration();

    $header = [
      [
        'data' => t('Content type'),
      ],
      [
        'data' => t('Operations'),
        'colspan' => 3,
      ],
    ];
    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];

    $rows = [];

    foreach ($types as $type => $object) {
      // Config data says to include the type.
      if ((!array_key_exists($type, $config['total_control_admin_types'])) || (isset($config['total_control_admin_types']) && $config['total_control_admin_types'][$type]) == $type) {
        // Check access, then add a link to create content.
        if ($access) {
          $rows[] = [
            'data' => [
              $object->get('name'),
              \Drupal::l('Configure', new Url('field_ui.field_storage_config_add_node', [
                'node_type' => $object->get('type'),
                'options' => $options,
              ])),
              \Drupal::l('Manage fields', new Url('entity.node.field_ui_fields', [
                'node_type' => $object->get('type'),
                'options' => $options,
              ])),
              \Drupal::l('Manage display', new Url('entity.entity_view_display.node.default', [
                'node_type' => $object->get('type'),
                'options' => $options,
              ])),
            ],
          ];
        }
      }
    }

    if (empty($rows)) {
      $rows[] = [
        [
          'data' => t('There are no content types to display.'),
          'colspan' => 4,
        ],
      ];
    }

    $link = '';
    if (\Drupal::currentUser()->hasPermission('administer content types')) {
      $link = \Drupal::l('Content type administration', new Url('entity.node_type.collection'));
    }

    $body_data = [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#footer' => $link,
    ];

    $table = drupal_render($body_data);

    return [
      '#type' => 'markup',
      '#markup' => $table . $link,
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form = parent::blockForm($form, $form_state);

    $config = $this->getConfiguration();
    $types = node_type_get_types();
    $type_defaults = [];

    foreach ($types as $type => $object) {
      $type_options[$type] = $object->get('name');
      if (!array_key_exists($type, $type_defaults)) {
        $type_defaults[$type] = $type;
      }
    }

    $form['total_control_admin_types'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Include Create links for Content Types'),
      '#options' => $type_defaults,
      '#default_value' => $config['total_control_admin_types'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_admin_types'] = $values['total_control_admin_types'];
  }

}
