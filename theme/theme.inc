<?php

/**
 * @file
 * Theme functions for the Islandora Solr Search module.
 */

use Drupal\Core\Url;
use Drupal\Core\Link;

/**
 * Prepares variables for islandora_solr_wrapper templates.
 *
 * @param array $variables
 *   An associative array containing:
 *   - results: Rendered primary display.
 *   - secondary_profiles: (optional) Rendered list of secondary display
 *     profiles.
 *   - elements: additional elements to populate the results page.
 *     - pager: (optional) Rendered pager.
 *     - solr_total: (optional) Total amount of documents found.
 *     - solr_start: (optional) Solr start position.
 *     - solr_end: (optional) Solr end position.
 *
 * @global string $base_url
 *
 * @see islandora_solr_theme()
 * @see IslandoraSolrResults::displayResults()
 */
function template_preprocess_islandora_solr_wrapper(array &$variables) {
  global $base_url;
  $variables['base_url'] = $base_url;

  $elements = $variables['elements'];

  // Make all variables in 'elements' available as variables in the template
  // file.
  foreach ($variables['elements'] as $key => $value) {
    $variables[$key] = $value;
  }

  // Results count.
  $total = $elements['solr_total'];
  $end = $elements['solr_end'];
  $start = $elements['solr_start'] + ($total > 0 ? 1 : 0);

  // Format numbers.
  $total = number_format($total, 0, '.', ',');
  $end = number_format($end, 0, '.', ',');
  $start = number_format($start, 0, '.', ',');

  $variables['islandora_solr_result_count'] = t('(@start - @end of @total)', [
    '@start' => $start,
    '@end' => $end,
    '@total' => $total,
  ]);
}

/**
 * Prepares variables for islandora_solr templates.
 */
function template_preprocess_islandora_solr(&$variables) {
  $results = $variables['results'];
  foreach ($results as $key => $result) {
    $options = ['html' => TRUE];
    if (isset($result['object_label'])) {
      $options['attributes']['title'] = $result['object_label'];
    }
    if (isset($result['object_url_params'])) {
      $options['query'] = $result['object_url_params'];
    }
    if (isset($result['object_url_fragment'])) {
      $options['fragment'] = $result['object_url_fragment'];
    }
    // Thumbnail link.
    $image = [
      '#theme' => 'image',
      '#uri' => Url::fromUri($result['thumbnail_url'], ['query' => $result['thumbnail_url_params']])->toString(),
    ];
    if (isset($result['object_label'])) {
      $image['#alt'] = $result['object_label'];
    }
    $variables['results'][$key]['thumbnail'] = Link::fromTextAndUrl(
      \Drupal::service('renderer')->render($image),
      Url::fromUri($result['object_url'], $options)
    );
  }
}

/**
 * Prepares variables for islandora_solr_facet templates.
 */
function template_preprocess_islandora_solr_facet(&$variables) {
  if ($variables['hidden']) {
    $variables['classes_array'][] = 'hidden';
  }
  $variables['classes_array'][] = 'islandora-solr-facet';
  $variables['classes'] = implode(' ', $variables['classes_array']);
}

/**
 * Implements hook_preprocess_islandora_solr_search_navigation_block().
 */
function template_preprocess_islandora_solr_search_navigation_block(&$variables) {
  if ($variables['prev_link']) {
    $variables['prev_link'] = format_string('<a href="@link">@text</a>', [
      '@link' => $variables['prev_link'],
      '@text' => $variables['prev_text'],
    ]);
  }
  $variables['return_link'] = format_string('<a href="@link">@text</a>', [
    '@link' => $variables['return_link'],
    '@text' => $variables['return_text'],
  ]);
  if ($variables['next_link']) {
    $variables['next_link'] = format_string('<a href="@link">@text</a>', [
      '@link' => $variables['next_link'],
      '@text' => $variables['next_text'],
    ]);
  }
}
