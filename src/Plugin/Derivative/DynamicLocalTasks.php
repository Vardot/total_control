<?php

namespace Drupal\total_control\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DeriverBase;

/**
 * Defines dynamic local tasks.
 */
class DynamicLocalTasks extends DeriverBase {

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $moduleHandler = \Drupal::service('module_handler');
    $router = \Drupal::service('router.route_provider');

    if ($moduleHandler->moduleExists('comment') && !empty($router->getRoutesByNames(['view.control_comments.page_1']))) {
      $this->derivatives['total_control.comments'] = $base_plugin_definition;
      $this->derivatives['total_control.comments']['title'] = t('Comments');
      $this->derivatives['total_control.comments']['base_route'] = 'page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0';
      $this->derivatives['total_control.comments']['route_name'] = 'view.control_comments.page_1';

      return $this->derivatives;
    }

    if ($moduleHandler->moduleExists('taxonomy') && !empty($router->getRoutesByNames(['view.control_terms.page_1']))) {
      $this->derivatives['total_control.categories'] = $base_plugin_definition;
      $this->derivatives['total_control.categories']['title'] = t('Categories');
      $this->derivatives['total_control.categories']['base_route'] = 'page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0';
      $this->derivatives['total_control.categories']['route_name'] = 'view.control_terms.page_1';

      return $this->derivatives;
    }
  }

}
