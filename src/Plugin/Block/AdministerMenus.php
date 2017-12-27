<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'Administer Menus'.
 *
 * @Block(
 * id = "administer_menus",
 * admin_label = @Translation("Administer Menus"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerMenus extends BlockBase implements BlockPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $items = [];

    $menus = menu_ui_get_menus();
    $config = $this->getConfiguration();

    $header = [
      [
        'data' => t('Menu'),
      ],
      [
        'data' => t('Operations'),
        'colspan' => 2,
      ],
    ];

    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];

    foreach ($menus as $menu_name => $menu) {
      $is_new = !array_key_exists($menu_name, $config['total_control_admin_menus']);
      if ($is_new || array_key_exists($menu_name, $config['total_control_admin_menus'])) {
        if ($is_new || $config['total_control_admin_menus'][$menu_name] === $menu_name) {
          $rows[] = [
            'data' => [
              $menu,
              \Drupal::l('Configure', new Url('entity.menu.edit_form', [
                'menu' => $menu_name,
                'options' => $options,
              ])),
              \Drupal::l('Add new link', new Url('entity.menu.add_link_form', [
                'menu' => $menu_name,
                'options' => $options,
              ])),
            ],
          ];
        }
      }
    }

    // Build a link to the menu admin UI.
    $link = '';
    if (\Drupal::currentUser()->hasPermission('administer menu')) {
      $link = \Drupal::l('Menu administration', new Url('entity.menu.collection'));
    }

    if (empty($rows)) {
      $rows[] = [
        [
          'data' => t('There are no menus to display.'),
          'colspan' => 3,
        ],
      ];
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
    $menus = menu_ui_get_menus();

    $form['total_control_admin_menus'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Show links for the following menus on the dashboard'),
      '#options' => $menus,
      '#default_value' => $config['total_control_admin_menus'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_admin_menus'] = $values['total_control_admin_menus'];
  }

}
