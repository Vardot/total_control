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

    if (\Drupal::service('module_handler')->moduleExists('page_manager_ui')) {

      $markup_data = '<p>' . $this->t('Welcome to your administrative dashboard.')
        . '&nbsp;<a href="/admin/structure/page_manager/manage/total_control_dashboard/page_variant__total_control_dashboard-http_status_code-0__content?js=nojs">'
        . $this->t('Edit this panel')
        . '</a>&nbsp;'
        . $this->t('to add more blocks here, or configure those provided by default.')
        . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }
    else {

      $markup_data = '<p>' .$this->t('Welcome to your administrative dashboard.')
        . '&nbsp;' . $this->t('You have to enable')
        . ' <strong>page manager ui</strong> '
        . $this->t('module to edit this panel.')
        . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }
  }

}
