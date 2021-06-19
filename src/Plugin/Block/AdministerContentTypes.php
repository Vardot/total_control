<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Link;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\StringTranslation\TranslationInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\Core\Render\RenderableInterface;
use Drupal\Core\Routing\RedirectDestinationInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Administer Content Types'.
 *
 * @Block(
 * id = "administer_content_types",
 * admin_label = @Translation("Administer Content Types"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerContentTypes extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

  /**
   * The module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * The database connection.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $connection;

  /**
   * The translation manager.
   *
   * @var \Drupal\Core\StringTranslation\TranslationInterface
   */
  protected $stringTranslation;

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * The renderer service.
   *
   * @var \Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * The redirect destination service.
   *
   * @var \Drupal\Core\Routing\RedirectDestinationInterface
   */
  protected $redirectDestination;

  /**
   * The current user service.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected $currentUser;

  /**
   * Creates an AdministerContentTypes block instance.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler service.
   * @param \Drupal\Core\Database\Connection $connection
   *   The database connection.
   * @param \Drupal\Core\StringTranslation\TranslationInterface $string_translation
   *   The translation manager.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   * @param \Drupal\Core\Routing\RedirectDestinationInterface $redirect_destination
   *   The redirect destination service.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   The current user service.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModuleHandlerInterface $module_handler, Connection $connection, TranslationInterface $string_translation, EntityTypeManagerInterface $entity_type_manager, RendererInterface $renderer, RedirectDestinationInterface $redirect_destination, AccountInterface $current_user) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->connection = $connection;
    $this->stringTranslation = $string_translation;
    $this->entityTypeManager = $entity_type_manager;
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
      $container->get('database'),
      $container->get('string_translation'),
      $container->get('entity_type.manager'),
      $container->get('renderer'),
      $container->get('redirect.destination'),
      $container->get('current_user')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    if (!$this->moduleHandler->moduleExists('field_ui')) {

      $markup_data = $this->t('You have to enable')
        . ' <strong>' . $this->t('Field UI') . '</strong> '
        . $this->t('module to see this block.');

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $types = $this->entityTypeManager->getStorage('node_type')->loadMultiple();
    $access = $this->currentUser->hasPermission('administer content types');
    $config = $this->getConfiguration();

    $header = [
      [
        'data' => $this->t('Content type'),
      ],
      [
        'data' => $this->t('Operations'),
        'colspan' => 3,
      ],
    ];
    $destination = $this->redirectDestination->getAsArray();
    $options = [
      $destination,
    ];

    $rows = [];

    foreach ($types as $type => $object) {
      // Config data says to include the type.
      if ((!array_key_exists($type, $config['total_control_admin_types']))
         || (isset($config['total_control_admin_types'])
         && $config['total_control_admin_types'][$type]) == $type) {

        // Check access, then add a link to create content.
        if ($access) {
          $rows[] = [
            'data' => [
              $object->get('name'),
              Link::fromTextAndUrl($this->t('Configure'),
                new Url('field_ui.field_storage_config_add_node', [
                  'node_type' => $object->get('type'),
                  'options' => $options,
                ]))->toString(),
              Link::fromTextAndUrl($this->t('Manage fields'),
                new Url('entity.node.field_ui_fields', [
                  'node_type' => $object->get('type'),
                  'options' => $options,
                ]))->toString(),
              Link::fromTextAndUrl($this->t('Manage display'),
                new Url('entity.entity_view_display.node.default', [
                  'node_type' => $object->get('type'),
                  'options' => $options,
                ]))->toString(),
            ],
          ];
        }
      }
    }

    if (empty($rows)) {
      $rows[] = [
        [
          'data' => $this->t('There are no content types to display.'),
          'colspan' => 4,
        ],
      ];
    }

    $link = NULL;
    if ($this->currentUser->hasPermission('administer content types')) {
      $link = Link::fromTextAndUrl($this->t('Content type administration'), new Url('entity.node_type.collection'));
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
    $types = $this->entityTypeManager->getStorage('node_type')->loadMultiple();
    $type_defaults = [];

    foreach ($types as $type => $object) {
      if (!array_key_exists($type, $type_defaults)) {
        $type_defaults[$type] = $type;
      }
    }

    $form['total_control_admin_types'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Include Create links for Content Types'),
      '#options' => $type_defaults,
      '#default_value' => $config['total_control_admin_types'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_admin_types'] = $values['total_control_admin_types'];
  }

}
