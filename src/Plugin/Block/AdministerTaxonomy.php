<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Link;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\StringTranslation\TranslationInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\Core\Render\RenderableInterface;
use Drupal\Core\Routing\RedirectDestinationInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Administer Taxonomy'.
 *
 * @Block(
 * id = "administer_taxonomy",
 * admin_label = @Translation("Administer Taxonomy"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerTaxonomy extends BlockBase implements BlockPluginInterface, ContainerFactoryPluginInterface {

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
   * Creates an AdministerTaxonomy block instance.
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
   * @param Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   * @param \Drupal\Core\Routing\RedirectDestinationInterface $redirect_destination
   *   The redirect destination service.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   The current user.
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

    if (!$this->moduleHandler->moduleExists('taxonomy')) {

      $markup_data = $this->t('You have to enable')
        . ' <strong>' . $this->t('Taxonomy') . '</strong> '
        . $this->t('module to see this block.');

      return [
        '#type' => 'markup',
        '#markup' => $markup_data,
      ];
    }

    $vocabs = $this->entityTypeManager->getStorage('taxonomy_vocabulary')->loadMultiple();
    $config = $this->getConfiguration();
    $vids = $config['total_control_admin_taxonomy'];
    $header = [
      [
        'data' => $this->t('Vocabulary'),
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
    if (!empty($vocabs)) {
      foreach ($vocabs as $key => $vocab) {
        if ((in_array($vocab->get('vid'), $vids)
          && isset($vids[$key])
          && $vids[$key] === $vocab->id())
          || !array_key_exists($vocab->get('vid'), $config['total_control_admin_taxonomy'])) {

          $term_query = $this->connection->query("SELECT count(*) FROM {taxonomy_term_data} WHERE vid = :vid", [
            ':vid' => $vocab->get('vid'),
          ]);

          $term_count = $term_query->fetchField();

          if ($this->currentUser->hasPermission('administer taxonomy')
             || $this->currentUser->hasPermission('edit terms in ' . $vocab->get('vid'))) {

            $terms = $this->stringTranslation->formatPlural($term_count, '1 categories', '@count categories');
            $rows[] = [
              'data' => [
                $vocab->get('name') . ': ' . $terms,
                Link::fromTextAndUrl($this->t('Configure'),
                  new Url('entity.taxonomy_vocabulary.edit_form', [
                    'taxonomy_vocabulary' => $vocab->get('vid'),
                    'options' => $options,
                  ]))->toString(),
                Link::fromTextAndUrl($this->t('Manage categories'),
                  new Url('entity.taxonomy_vocabulary.overview_form', [
                    'taxonomy_vocabulary' => $vocab->get('vid'),
                    'options' => $options,
                  ]))->toString(),
                Link::fromTextAndUrl($this->t('Add new category'),
                  new Url('entity.taxonomy_term.add_form', [
                    'taxonomy_vocabulary' => $vocab->get('vid'),
                    'options' => $options,
                  ]))->toString(),
              ],
            ];
          }
        }
      }
    }

    if (empty($rows)) {
      $rows[] = [
        'data' => $this->t('There are no vocabularies to display.'),
        'colspan' => 4,
      ];
    }

    $link = NULL;
    if ($this->currentUser->hasPermission('administer taxonomy')) {
      $link = Link::fromTextAndUrl($this->t('Taxonomy administration'),
      new Url('entity.taxonomy_vocabulary.collection', $options));
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
    $vocabularies = $this->entityTypeManager
      ->getStorage('taxonomy_vocabulary')
      ->loadMultiple();

    $vocabularies_defaults = [];

    foreach ($vocabularies as $vocabulary => $object) {
      if (!array_key_exists($vocabulary, $vocabularies_defaults)) {
        $vocabularies_defaults[$vocabulary] = $vocabulary;
      }
    }

    $form['total_control_admin_taxonomy'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Include Vocabularies'),
      '#options' => $vocabularies_defaults,
      '#default_value' => $config['total_control_admin_taxonomy'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    parent::blockSubmit($form, $form_state);
    $values = $form_state->getValues();
    $this->configuration['total_control_admin_taxonomy'] = $values['total_control_admin_taxonomy'];
  }

}
