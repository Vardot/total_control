<?php

/**
 * @file
 * Contains \Drupal\total_control\Plugin\Derivative\Menu\TotalControlMenuLink.
 */

namespace Drupal\total_control\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Represents a menu link for a single Product.
 */
class TotalControlMenuLink extends DeriverBase implements ContainerDeriverInterface {

  /**
   * @var EntityTypeManagerInterface $entityTypeManager.
   */
  protected $entityTypeManager;

  /**
   * Creates a ProductMenuLink instance.
   *
   * @param $base_plugin_id
   * @param EntityTypeManagerInterface $entity_type_manager
   */
  public function __construct($base_plugin_id, EntityTypeManagerInterface $entity_type_manager) {
    $this->entityTypeManager = $entity_type_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, $base_plugin_id) {
    return new static(
      $base_plugin_id, $container->get('entity_type.manager')
    );
  }

  /**
   *
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $router = \Drupal::service('router.route_provider');
    if (!empty($router->getRoutesByNames(array('page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0',)))) {
      $links ['system.total_control_dashboard'] = [
        'title' => 'Dashboard',
        'route_name' => 'page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0',
        'parent' => 'system.admin',
        'weight' => '-20'
        ] + $base_plugin_definition;
      return $links;
    }
  }

}
