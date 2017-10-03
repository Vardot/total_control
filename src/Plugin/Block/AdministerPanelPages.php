<?php

/**
 * @file
 * Contains \Drupal\total_control\Plugin\Block\AdministerPanelPages.
 */

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
   *
   * {@inheritdoc}
   */
  public function build() {
    $moduleHandler = \Drupal::service('module_handler');
    $pm_ui_exist = $moduleHandler->moduleExists('page_manager_ui');

    if (!$pm_ui_exist) {
      return array(
        '#type' => 'markup',
        '#markup' => '<p>You have to enable <strong>page manager ui</strong> module to see this block.</p>'
      );
    }
    $panels = \Drupal::entityTypeManager()->getStorage('page')->loadMultiple();
    $header = array(
      array(
        'data' => t('Page')
      ),
      array(
        'data' => t('Operations'),
        'colspan' => 2
      )
    );
    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];

    foreach ($panels as $panel) {
      $rows [] = array(
        'data' => array(
          $panel->get('label'),
          \Drupal::l('Edit', new Url('entity.page.edit_form', [
            'machine_name' => $panel->get('id'), 'step' => 'general',
            $options
          ])),
          \Drupal::l('Disable', new Url('entity.page.disable', [
            'page' => $panel->get('id'),
            $options
          ])),
        )
      );
    }

    $link = '';
    if (\Drupal::currentUser()->hasPermission('administer pages')) {
      $link = \Drupal::l('Page manager administration', new Url('entity.page.collection'));
    }

    $body_data = [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ];

    $table = drupal_render($body_data);
    return array(
      '#type' => 'markup',
      '#markup' => $table . $link
    );
  }

}
