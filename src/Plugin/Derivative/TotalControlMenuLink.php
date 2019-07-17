<?php

namespace Drupal\total_control\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\StringTranslation\TranslationInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Represents a menu link for a Total Control Menu Link.
 */
class TotalControlMenuLink extends DeriverBase implements ContainerDeriverInterface {

  use StringTranslationTrait;

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Creates a Total Control Menu Link instance.
   *
   * @param string $base_plugin_id
   *   Base plugin id.
   * @param Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   Entity type manager.
   * @param Drupal\Core\StringTranslation\TranslationInterface $string_translation
   *   The string translation service.
   */
  public function __construct($base_plugin_id, EntityTypeManagerInterface $entity_type_manager, TranslationInterface $string_translation) {
    $this->entityTypeManager = $entity_type_manager;
    $this->setStringTranslation($string_translation);
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, $base_plugin_id) {
    return new static(
      $base_plugin_id,
      $container->get('entity_type.manager'),
      $container->get('string_translation')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $router = \Drupal::service('router.route_provider');
    if (!empty($router->getRoutesByNames(['page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0']))) {
      $links['system.total_control_dashboard'] = [
        'title' => $this->t('Dashboard'),
        'route_name' => 'page_manager.page_view_total_control_dashboard_total_control_dashboard-http_status_code-0',
        'parent' => 'system.admin',
        'weight' => '-20',
      ] + $base_plugin_definition;

      return $links;
    }

    return [];
  }

}
