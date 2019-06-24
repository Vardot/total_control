<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;

/**
 * Provides a 'Administer Panel Pages'.
 *
 * @Block(
 * id = "administer_panel_pages",
 * admin_label = @Translation("Administer Panel Pages"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerPanelPages extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {

    if (!$moduleHandler->moduleExists('page_manager_ui')) {

      $markup_data = '<p>' . $this->t('You have to enable')
        . ' <strong>page manager ui</strong> '
        . $this->t('module to see this block.')
        . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $panels = \Drupal::entityTypeManager()->getStorage('page')->loadMultiple();
    $header = [
      [
        'data' => $this->t('Page'),
      ],
      [
        'data' => $this->t('Operations'),
        'colspan' => 2,
      ],
    ];
    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];

    foreach ($panels as $panel) {
      $rows[] = [
        'data' => [
          $panel->get('label'),
          \Drupal::l('Edit', new Url('entity.page.edit_form', [
            'machine_name' => $panel->get('id'),
            'step' => 'general',
            'options' => $options,
          ])),
          \Drupal::l('Disable', new Url('entity.page.disable', [
            'page' => $panel->get('id'),
            'options' => $options,
          ])),
        ],
      ];
    }

    $link = '';
    if (\Drupal::currentUser()->hasPermission('administer pages')) {
      $link = \Drupal::l($this->t('Page manager administration'), new Url('entity.page.collection'));
    }

    $body_data = [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ];

    $table = drupal_render($body_data);
    return [
      '#type' => 'markup',
      '#markup' => $table . $link,
    ];
  }

}
