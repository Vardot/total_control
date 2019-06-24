<?php

namespace Drupal\total_control\Plugin\Block;

use Drupal\Core\Url;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Block\BlockPluginInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'Content Overview'.
 *
 * @Block(
 * id = "content_overview",
 * admin_label = @Translation("Content Overview"),
 * category = @Translation("Dashboard")
 * )
 */
class ContentOverview extends BlockBase implements BlockPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $items = [];
    $types = node_type_get_types();
    $config = $this->getConfiguration();

    foreach ($types as $type => $object) {
      // Compare against type option on pane config.
      if ((!array_key_exists($type, $config['total_control_types_overview'])) || (isset($config['total_control_types_overview']) && $config['total_control_types_overview'][$type]) == $type) {
        $type_count = db_query("SELECT count(*) FROM {node_field_data} WHERE type = :type and status = 1", [
          ':type' => $type,
        ])->fetchField();
        $content_data[$type] = \Drupal::translation()->formatPlural($type_count, '1 ' . $object->get('name') . ' item', '@count ' . $object->get('name') . ' items');

        // Check if comments module is enabled.
        if (\Drupal::service('module_handler')->moduleExists('comment')) {
          // Compare against comment options on pane config.
          if ((!array_key_exists($type, $config['total_control_comments_overview'])) || (isset($config['total_control_comments_overview']) && $config['total_control_comments_overview'][$type]) == $type) {
            $comment_count = db_query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {comment_field_data} n ON c.cid = n.cid INNER JOIN {node} node WHERE n.entity_id = node.nid AND node.type = :type AND n.status = 1", [
              ':type' => $type,
            ])->fetchField();
            $content_data[$type . '_comments'] = \Drupal::translation()->formatPlural($comment_count, '1 comment', '@count comments');

            // Compare against spam option checkbox on pane config.
            if (isset($config['total_control_spam_overview']) && $config['total_control_spam_overview'] == 1) {
              $spam_count = db_query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {comment_field_data} n ON c.cid = n.cid INNER JOIN {node} node WHERE n.entity_id = node.nid AND node.type = :type AND n.status = 0", [
                ':type' => $type,
              ])->fetchField();
              $content_data[$type . '_comments_spam'] = \Drupal::translation()->formatPlural($spam_count, '1 spam', '@count spam');
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

      $markup_data = $this->t('No content available. ') 
        . \Drupal::l($this->t('Add content'), new Url('node.add_page'));

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

    return [
      '#type' => 'markup',
      '#markup' => drupal_render($body_data),
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form = parent::blockForm($form, $form_state);

    $config = $this->getConfiguration();
    $types = node_type_get_types();
    $type_defaults = [];

    foreach ($types as $type => $object) {
      $type_options[$type] = $object->get('name');
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
