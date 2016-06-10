<?php

/**
 * @file
 * Handles the isMemberOfSite values for objects.
 */

/**
 * The add memberofsite form.
 */
function upitt_islandora_memberofsite_memberofsite_add($form, &$form_state, AbstractObject $object) {
  $sites = variable_get('upitt_islandora_memberofsite_sites');
  // Remove the sites that have been configured for this object already.
  $obj_sites = $object->relationships->get(NULL, 'isMemberOfSite');
  $existing_sites = $newsites = array();
  if (count($obj_sites) > 0) {
    foreach ($obj_sites as $obj_site) {
      if (isset($obj_site['object']['value'])) {
        $existing_sites[] = $obj_site['object']['value'];
      }
    }
  }
  $sites_arr = explode("\r\n", $sites);
  foreach ($sites_arr as $site) {
    if (($site) && (array_search($site, $existing_sites) === FALSE)) {
      $newsites[$site] = $site;
    }
  }
  if (count($newsites) < 1) {
    drupal_set_message('This object is already associated with all possible site values or no sites have been configured for the isMemberOfSite module.');
    drupal_goto("islandora/object/{$object->id}/manage/");
    return FALSE;
  }
  $form = array(
    'site' => array(
      '#type' => 'select',
      '#title' => t('Add IsMemberOfSite'),
      '#description' => t('Select the rdf:resource attribute for the new IsMemberOfSite association.'),
      '#options' => $newsites,
    ),
    'object_id' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  return $form;
}

/**
 * Submit handler for upitt_islandora_memberofsite_memberofsite_add.
 */
function upitt_islandora_memberofsite_memberofsite_add_submit($form, &$form_state) {
  $object = islandora_object_load($form_state['values']['object_id']);
  drupal_set_message(t('The "isMemberOfSite" relationship has been added to "@object_id".', array('object_id' => $form_state['values']['object_id'])));
  $form_state['redirect'] = "islandora/object/{$form_state['values']['object_id']}/manage/";
}

/**
 * The remove memberofsite handler.
 */
function upitt_islandora_memberofsite_memberofsite_remove($form, &$form_state, AbstractObject $object, $ns, $site) {
  $form = array();
  $ns = urldecode($ns);
  $site = urldecode($site);
  $form = array(
    'ns' => array(
      '#type' => 'value',
      '#value' => $ns,
    ),
    'site' => array(
      '#type' => 'value',
      '#value' => $site,
    ),
    'object_id' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the "isMemberOfSite" relationship from "@site" in the "@ns" resource?', array('site' => $site, 'ns' => $ns)),
    "islandora/object/{$object->id}/manage/",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for upitt_islandora_memberofsite_memberofsite_remove.
 */
function upitt_islandora_memberofsite_memberofsite_remove_submit($form, &$form_state) {
  $ns = $form_state['values']['ns'];
  $site = $form_state['values']['site'];
  if (isset($form_state['values']['ns']) && isset($form_state['values']['object_id'])) {
    $object = islandora_object_load($form_state['values']['object_id']);
    // Remove the relationship that matches the string value for $ns / $site.
    if ($object->relationships->remove($ns, 'isMemberOfSite', $form_state['values']['site'])) {
      drupal_set_message(t('The "isMemberOfSite" relationship has been removed for "@site" in the "@ns" resource.', array('site' => $site, 'ns' => $ns)));
    }
    else {
      drupal_set_message(t('There was a problem attempting to remove the "isMemberOfSite" relationship for "@site" in the "@ns" resource.', array('site' => $site, 'ns' => $ns)));
    }
  }
  $form_state['redirect'] = "islandora/object/{$object->id}/manage/";
}
