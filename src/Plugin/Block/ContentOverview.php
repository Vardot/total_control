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
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Content Overview'.
 *
 * @Block(
 * id = "content_overview",
 * admin_label = @Translation("Content Overview"),
 * category = @Translation("Dashboard")
 * )
 */
class ContentOverview extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

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
   * @var Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * Creates a ContentOverview block instance.
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
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModuleHandlerInterface $module_handler, Connection $connection, TranslationInterface $string_translation, EntityTypeManagerInterface $entity_type_manager, RendererInterface $renderer) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->connection = $connection;
    $this->stringTranslation = $string_translation;
    $this->entityTypeManager = $entity_type_manager;
    $this->renderer = $renderer;
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
      $container->get('renderer')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $items = [];
    $types = $this->entityTypeManager->getStorage('node_type')->loadMultiple();
    $config = $this->getConfiguration();

    foreach ($types as $type => $object) {
      // Compare against type option on pane config.
      if ((!array_key_exists($type, $config['total_control_types_overview']))
       || (isset($config['total_control_types_overview'])
       && $config['total_control_types_overview'][$type]) == $type) {

        $type_query = $this->connection->query("SELECT count(*) FROM {node_field_data} WHERE type = :type and status = 1", [
          ':type' => $type,
        ]);

        $type_count = $type_query->fetchField();

        $content_data[$type] = $this->stringTranslation->formatPlural($type_count, '1 ' . $object->get('name') . ' item', '@count ' . $object->get('name') . ' items');

        // Check if comments module is enabled.
        if ($this->moduleHandler->moduleExists('comment')) {
          // Compare against comment options on pane config.
          if ((!array_key_exists($type, $config['total_control_comments_overview']))
             || (isset($config['total_control_comments_overview'])
             && $config['total_control_comments_overview'][$type]) == $type) {

            $comment_query = $this->connection->query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {comment_field_data} n ON c.cid = n.cid INNER JOIN {node} node WHERE n.entity_id = node.nid AND node.type = :type AND n.status = 1", [
              ':type' => $type,
            ]);

            $comment_count = $comment_query->fetchField();

            $content_data[$type . '_comments'] = $this->stringTranslation->formatPlural($comment_count, '1 comment', '@count comments');

            // Compare against spam option checkbox on pane config.
            if (isset($config['total_control_spam_overview']) && $config['total_control_spam_overview'] == 1) {

              $spam_query = $this->connection->query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {comment_field_data} n ON c.cid = n.cid INNER JOIN {node} node WHERE n.entity_id = node.nid AND node.type = :type AND n.status = 0", [
                ':type' => $type,
              ]);

              $spam_count = $spam_query->fetchField();

              $content_data[$type . '_comments_spam'] = $this->stringTranslation->formatPlural($spam_count, '1 spam', '@count spam');
            }
          }
        }

        $line = $content_data[$type];
        $line .= (isset($content_data[$type . '_comments'])) ? ' with ' . $content_data[$type . '_comments'] : '';
        $line .= (isset($content_data[$type . '_comments_spam'])) ? ' (' . $content_data[$type . '_comments_spam'] . ')' : '';
        $items[] = $line;
      }
    }

    if (empty($items)) {

      $markup_data = $this->t('No content available.') . ' '
        . Link::fromTextAndUrl($this->t('Add content'),
        new Url('node.add_page'))->toString();

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $body_data = [
      '#theme' => 'item_list',
      '#list_type' => 'ul',
      '#items' => $items,
    ];

    $markup_data = $this->renderer->render($body_data);

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

    $form['total_control_types_overview'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Show post counts for the following content types'),
      '#options' => $type_defaults,
      '#default_value' => $config['total_control_types_overview'],
    ];

    $form['total_control_comments_overview'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Show comment counts for the following content types'),
      '#options' => $type_defaults,
      '#default_value' => $config['total_control_comments_overview'],
    ];

    $spam_options = [
      0 => $this->t('no'),
      1 => $this->t('Yes'),
    ];

    $form['total_control_spam_overview'] = [
      '#type' => 'radios',
      '#title' => $this->t('Include spam counts with comments'),
      '#options' => $spam_options,
      '#default_value' => $config['total_control_spam_overview'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_types_overview'] = $values['total_control_types_overview'];
    $this->configuration['total_control_comments_overview'] = $values['total_control_comments_overview'];
    $this->configuration['total_control_spam_overview'] = $values['total_control_spam_overview'];
  }

}
