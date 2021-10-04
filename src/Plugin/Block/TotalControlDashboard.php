<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Provides a 'Total Control Dashboard block'.
 *
 * @Block(
 * id = "total_control_dashboard",
 * admin_label = @Translation("Take Total Control."),
 * category = @Translation("Dashboard")
 * )
 */
class TotalControlDashboard extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

  /**
   * The module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * The current Request object.
   *
   * @var \Symfony\Component\HttpFoundation\Request
   */
  protected $request;

  /**
   * Creates a CreateContent block instance.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler service.
   * @param \Symfony\Component\HttpFoundation\Request $request
   *   The current request.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModuleHandlerInterface $module_handler, Request $request) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->request = $request;
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
      $container->get('request_stack')->getCurrentRequest()
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    if ($this->moduleHandler->moduleExists('page_manager_ui')) {
      $welcome_message_text = $this->t('Welcome to your administrative dashboard.');
      $edit_this_panel_text = $this->t('Edit this panel');
      $to_do_more_text = $this->t('to add more blocks here, or configure those provided by default.');

      // Until #1088112: Introduce a token to get site's base URL is
      // committed,
      // https://www.drupal.org/project/drupal/issues/1088112
      // let's use a custom token. Let's call it: [site:origin-url].
      // No language prefix in the url.
      // -----------------------------------------------------------------.
      /** @var \Symfony\Component\HttpFoundation\Request $origin_url */
      $origin_url = $this->request->getSchemeAndHttpHost() . $this->request->getBaseUrl();

      $markup_data = '<p>' . $welcome_message_text
        . ' <a href="' . $origin_url . '/admin/structure/page_manager/manage/total_control_dashboard/page_variant__total_control_dashboard-http_status_code-0__content?js=nojs">'
        . $edit_this_panel_text . '</a> ' . $to_do_more_text . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }
    else {
      $welcome_to_dashboard_text = $this->t('Welcome to your administrative dashboard.');
      $you_have_to_enable_text = $this->t('You have to enable');
      $page_manager_ui_text = $this->t('Page Manager UI');
      $to_edit_text = $this->t('module to edit this panel.');

      $markup_data = '<p>' . $welcome_to_dashboard_text
        . '&nbsp;' . $you_have_to_enable_text
        . ' <strong>' . $page_manager_ui_text . '</strong> '
        . $to_edit_text . '</p>';

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }
  }

}
