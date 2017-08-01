<?php

/**
 * @file
 * Contains \Drupal\total_control\Plugin\Block\TotalControlDashboard.
 */

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
   *
   * {@inheritdoc}
   */
  public function build() {
    return array(
      '#type' => 'markup',
      '#markup' => '<p>Welcome to your administrative dashboard.&nbsp;<a href="/admin/structure/page_manager/manage/total_control_dashboard/page_variant__total_control_dashboard-http_status_code-0__content?js=nojs">Edit this panel</a>&nbsp;to add more blocks here, or configure those provided by default.</p>'
    );
  }

}
