<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'Create content'.
 *
 * @Block(
 * id = "create_content",
 * admin_label = @Translation("Create Content"),
 * category = @Translation("Dashboard")
 * )
 */
class CreateContent extends BlockBase implements BlockPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $types = node_type_get_types();
    $links = [];
    $config = $this->getConfiguration();
    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];
    foreach ($types as $type => $object) {
      // Check against pane config for type.
      if ((!array_key_exists($type, $config['total_control_admin_types_links'])) || (isset($config['total_control_admin_types_links']) && $config['total_control_admin_types_links'][$type]) == $type) {
        // Check access, then add a link to create content.
        if (\Drupal::currentUser()->hasPermission('create ' . $object->get('type') . ' content')) {
          $links[] = \Drupal::l('Add new  ' . $object->get('name'), new Url('node.add', [
            'node_type' => $object->get('type'),
            'options' => $options,
          ]));
        }
      }
    }

    $body_data = [
      '#theme' => 'item_list',
      '#list_type' => 'ul',
      '#items' => $links,
    ];

    return [
      '#type' => 'markup',
      '#markup' => drupal_render($body_data),
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

    $form['total_control_admin_types_links'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Include Create links for Content Types'),
      '#options' => $type_defaults,
      '#default_value' => $config['total_control_admin_types_links'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_admin_types_links'] = $values['total_control_admin_types_links'];
  }

}
