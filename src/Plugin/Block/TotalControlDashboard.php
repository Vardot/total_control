<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'Total Control Dashboard block'.
 *
 * @Block(
 * id = "total_control_dashboard",
 * admin_label = @Translation("Take Total Control."),
 * category = @Translation("Dashboard")
 * )
 */
class TotalControlDashboard extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $moduleHandler = \Drupal::service('module_handler');
    $pm_ui_exist = $moduleHandler->moduleExists('page_manager_ui');

    if ($pm_ui_exist) {
      return [
        '#type' => 'markup',
        '#markup' => t('<p>Welcome to your administrative dashboard.&nbsp;<a href="/admin/structure/page_manager/manage/total_control_dashboard/page_variant__total_control_dashboard-http_status_code-0__content?js=nojs">Edit this panel</a>&nbsp;to add more blocks here, or configure those provided by default.</p>'),
      ];
    }
    else {
      return [
        '#type' => 'markup',
        '#markup' => t('<p>Welcome to your administrative dashboard.&nbsp;You have to enable <strong>page manager ui</strong> module to edit this panel.</p>'),
      ];
    }
  }

}
