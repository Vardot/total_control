<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Link;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\Core\Render\RenderableInterface;
use Drupal\Core\Routing\RedirectDestinationInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Administer Panel Pages'.
 *
 * @Block(
 * id = "administer_panel_pages",
 * admin_label = @Translation("Administer Panel Pages"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerPanelPages extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

  /**
   * The module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * The renderer service.
   *
   * @var Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * The redirect destination service.
   *
   * @var \Drupal\Core\Routing\RedirectDestinationInterface
   */
  protected $redirectDestination;

  /**
   * The current user.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected $currentUser;

  /**
   * Creates an AdministerPanelPages block instance.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler service.
   * @param \Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\Routing\RedirectDestinationInterface $redirect_destination
   *   The redirect destination service.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   The current user.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModuleHandlerInterface $module_handler, RendererInterface $renderer, EntityTypeManagerInterface $entity_type_manager, RedirectDestinationInterface $redirect_destination, AccountInterface $current_user) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->renderer = $renderer;
    $this->entityTypeManager = $entity_type_manager;
    $this->redirectDestination = $redirect_destination;
    $this->currentUser = $current_user;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('module_handler'),
      $container->get('renderer'),
      $container->get('entity_type.manager'),
      $container->get('redirect.destination'),
      $container->get('current_user')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    if (!$this->moduleHandler->moduleExists('page_manager_ui')) {
      $you_have_to_enable_text = $this->t('You have to enable');
      $page_manager_ui_text = $this->t('Page Manager UI');
      $to_see_this_block_text = $this->t('module to see this block.');

      $markup_data = '<p>' . $you_have_to_enable_text
        . ' <strong>' . $page_manager_ui_text . '</strong> '
        . $to_see_this_block_text . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $panels = $this->entityTypeManager->getStorage('page')->loadMultiple();
    $header = [
      [
        'data' => $this->t('Page'),
      ],
      [
        'data' => $this->t('Operations'),
        'colspan' => 2,
      ],
    ];
    $destination = $this->redirectDestination->getAsArray();
    $options = [
      $destination,
    ];

    foreach ($panels as $panel) {
      $rows[] = [
        'data' => [
          $panel->get('label'),
          Link::fromTextAndUrl($this->t('Edit'),
            new Url('entity.page.edit_form', [
              'machine_name' => $panel->get('id'),
              'step' => 'general',
              'options' => $options,
            ]))->toString(),
          Link::fromTextAndUrl($this->t('Disable'),
            new Url('entity.page.disable', [
              'page' => $panel->get('id'),
              'options' => $options,
            ]))->toString(),
        ],
      ];
    }

    $link = NULL;
    if ($this->currentUser->hasPermission('administer pages')) {
      $link = Link::fromTextAndUrl($this->t('Page manager administration'),
        new Url('entity.page.collection'));
    }

    $body_data = [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ];

    $markup_data = $this->renderer->render($body_data);
    if ($link instanceof RenderableInterface) {
      $markup_data .= $link->toString();
    }

    return [
      '#type' => 'markup',
      '#markup' => $markup_data,
    ];
  }

}
