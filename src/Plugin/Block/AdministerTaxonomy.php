<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\taxonomy\Entity\Vocabulary;

/**
 * Provides a 'Administer Taxonomy'.
 *
 * @Block(
 * id = "administer_taxonomy",
 * admin_label = @Translation("Administer Taxonomy"),
 * category = @Translation("Dashboard")
 * )
 */
class AdministerTaxonomy extends BlockBase implements BlockPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $vocabs = Vocabulary::loadMultiple();
    $config = $this->getConfiguration();
    $vids = $config['total_control_admin_taxonomy'];
    $header = [
      [
        'data' => t('Vocabulary'),
      ],
      [
        'data' => t('Operations'),
        'colspan' => 3,
      ],
    ];
    $destination = drupal_get_destination();
    $options = [
      $destination,
    ];
    $rows = [];
    if (!empty($vocabs)) {
      foreach ($vocabs as $key => $vocab) {
        if ((in_array($vocab->get('vid'), $vids) && isset($vids[$key]) && $vids[$key] == $vocab->get('name')) || !array_key_exists($vocab->get('vid'), $config['total_control_admin_taxonomy'])) {
          $term_count = db_query("SELECT count(*) FROM {taxonomy_term_data} WHERE vid = :vid", [
            ':vid' => $vocab->get('vid'),
          ])->fetchField();
          if (\Drupal::currentUser()->hasPermission('administer taxonomy') || \Drupal::currentUser()->hasPermission('edit terms in ' . $vocab->get('vid'))) {
            $terms = \Drupal::translation()->formatPlural($term_count, '1 categories', '@count categories');
            $rows[] = [
              'data' => [
                $vocab->get('name') . ': ' . $terms,
                \Drupal::l('Configure', new Url('entity.taxonomy_vocabulary.edit_form', [
                  'taxonomy_vocabulary' => $vocab->get('vid'),
                  'options' => $options,
                ])),
                \Drupal::l('Manage categories', new Url('entity.taxonomy_vocabulary.overview_form', [
                  'taxonomy_vocabulary' => $vocab->get('vid'),
                  'options' => $options,
                ])),
                \Drupal::l('Add new category', new Url('entity.taxonomy_term.add_form', [
                  'taxonomy_vocabulary' => $vocab->get('vid'),
                  'options' => $options,
                ])),
              ],
            ];
          }
        }
      }
    }

    if (empty($rows)) {
      $rows[] = [
        'data' => t('There are no vocabularies to display.'),
        'colspan' => 4,
      ];
    }

    $link = '';
    if (\Drupal::currentUser()->hasPermission('administer taxonomy')) {
      $link = \Drupal::l('Taxonomy administration', new Url('entity.taxonomy_vocabulary.collection', $options));
    }

    $body_data = [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    ];

    $table = drupal_render($body_data);
    return [
      '#type' => 'markup',
      '#markup' => $table . $link,
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form = parent::blockForm($form, $form_state);

    $config = $this->getConfiguration();
    $vocabularies = Vocabulary::loadMultiple();
    $vocabularies_defaults = [];

    foreach ($vocabularies as $vocabulary => $object) {
      $$vocabulary_options[$type] = $object->get('name');
      if (!array_key_exists($$vocabulary, $vocabularies_defaults)) {
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
