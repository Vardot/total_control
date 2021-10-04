<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Link;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\Core\Render\RenderableInterface;
use Drupal\Core\Routing\RedirectDestinationInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Administer Menus'.
 *
 * @Block(
 * id = "administer_menus",
 * admin_label = @Translation("Administer Menus"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerMenus extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

  /**
   * The module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

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
   * Creates an AdministerMenus block instance.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler service.
   * @param Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   * @param \Drupal\Core\Routing\RedirectDestinationInterface $redirect_destination
   *   The redirect destination service.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   The current user.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModuleHandlerInterface $module_handler, RendererInterface $renderer, RedirectDestinationInterface $redirect_destination, AccountInterface $current_user) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->renderer = $renderer;
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
      $container->get('redirect.destination'),
      $container->get('current_user')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    if (!$this->moduleHandler->moduleExists('menu_ui')) {
      $you_have_to_enable_text = $this->t('You have to enable');
      $menu_ui_text = $this->t('Menu UI');
      $to_see_this_block_text = $this->t('module to see this block.');

      $markup_data = $you_have_to_enable_text
        . ' <strong>' . $menu_ui_text . '</strong> '
        . $to_see_this_block_text;

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $menus = menu_ui_get_menus();

    // Support the custom permissions from the "menu_admin_per_menu" module.
    if ($this->moduleHandler->moduleExists('menu_admin_per_menu')) {
      if (!$this->currentUser->hasPermission('administer menu')) {
        /** @var \Drupal\menu_admin_per_menu\MenuAdminPerMenuAccessInterface $allowedMenusService */
        // phpcs:ignore
        $allowedMenusService = \Drupal::service('menu_admin_per_menu.allowed_menus');
        $allowed_menus = $allowedMenusService->getPerMenuPermissions($this->currentUser);
        foreach ($menus as $id => $label) {
          if (!in_array($id, $allowed_menus)) {
            unset($menus[$id]);
          }
        }
      }
    }

    $config = $this->getConfiguration();

    $header = [
      [
        'data' => $this->t('Menu'),
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

    foreach ($menus as $menu_name => $menu) {
      $is_new = !array_key_exists($menu_name, $config['total_control_admin_menus']);
      if ($is_new || array_key_exists($menu_name, $config['total_control_admin_menus'])) {
        if ($is_new || $config['total_control_admin_menus'][$menu_name] === $menu_name) {
          $rows[] = [
            'data' => [
              $menu,
              Link::fromTextAndUrl($this->t('Configure'),
                new Url('entity.menu.edit_form', [
                  'menu' => $menu_name,
                  'options' => $options,
                ]))->toString(),
              Link::fromTextAndUrl($this->t('Add new link'),
                new Url('entity.menu.add_link_form', [
                  'menu' => $menu_name,
                  'options' => $options,
                ]))->toString(),
            ],
          ];
        }
      }
    }

    // Build a link to the menu admin UI.
    $link = NULL;
    if ($this->currentUser->hasPermission('administer menu')) {
      $link = Link::fromTextAndUrl($this->t('Menu administration'),
      new Url('entity.menu.collection'));
    }

    if (empty($rows)) {
      $rows[] = [
        [
          'data' => $this->t('There are no menus to display.'),
          'colspan' => 3,
        ],
      ];
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
