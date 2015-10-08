<?php

function ammo_entity_check_title($type, $title) {
  // check if title is unique
  $query = new EntityFieldQuery();                                                                  
  $result = $query
    ->entityCondition('entity_type', $type)
    ->propertyCondition('title', $title)
    ->execute();
  if (!empty($result[$type])) {
    return FALSE;
  }
}

function ammo_error($message) {
  return '<section class="messages"><div class="messages error">'.$message.'</div></section>';
}

function ammo_amendment_states() {
  return array(
    'concept' => 'Concept',
    'submitted' => 'Ingediend',
    'accepted' => 'Overgenomen',
    'accepted_amended' => 'Aangepast overgenomen',
    'accepted_intention' => 'Strekking overgenomen',
    'rejected' => 'Afgewezen',
    'deleted' => 'Verwijderd',
  );
}

function ammo_motion_states() {
  return array(
    'concept' => 'Concept',
    'submitted' => 'Ingediend',
    'accepted' => 'Overgenomen',
    'accepted_amended' => 'Aangepast overgenomen',
    'accepted_intention' => 'Strekking overgenomen',
    'rejected' => 'Afgewezen',
    'deleted' => 'Verwijderd',
  );
}

function ammo_amendment_actions() {
  return array(
    'add' => 'Toevoegen tekst',
    'replace' => 'Vervangen tekst',
    'delete' => 'Schrappen tekst',
  );
}

function ammo_amendment_advice() {
  return array(
    'accept' => 'Overnemen',
    'accept_amended' => 'Gewijzigd overnemen',
    'accept_intention' => 'Strekking overnemen',
    'reject' => 'Afwijzen',
  );
}

function ammo_motion_advice() {
  return array(
    'accept' => 'Overnemen',
    'accept_amended' => 'Gewijzigd overnemen',
    'accept_intention' => 'Strekking overnemen',
    'reject' => 'Afwijzen',
  );
}

function ammo_get_afdelingen($contact_id = 'all') {
  $afdelingen = &drupal_static(__FUNCTION__);
  if (!isset($afdelingen[$contact_id])) {
    if ($cache = cache_get('ammo_data')) {
      $data = $cache->data;
      if (isset($data['afdelingen'][$contact_id])) {
        $afdelingen[$contact_id] = $data['afdelingen'][$contact_id];
      }
      else {
        $afdelingen[$contact_id] = ammo_get_afdelingen_helper($contact_id);
        $data['afdelingen'][$contact_id] = $afdelingen[$contact_id];
        cache_set('ammo_data', $data, 'cache', time() + 86400);
      }
    }
    else {
      $afdelingen[$contact_id] = ammo_get_afdelingen_helper($contact_id);
      $data['afdelingen'][$contact_id] = $afdelingen[$contact_id];
      cache_set('ammo_data', $data, 'cache', time() + 86400);
    }
  }
  return $afdelingen[$contact_id];
}

function ammo_get_afdelingen_helper($contact_id = NULL) {
  if ($contact_id == 'all') {
    $contact_id = NULL;
  }
  civicrm_initialize();
  $afdelingen_data = array();
  $relations = array(
    '97' => 'Voorzitter',
    '98' => 'Vervangend voorzitter',
  );
  foreach ($relations as $relationship_type_id => $relation_name) {
    // Get afdeling id.
    $params = array(
      'version' => 3,
      'sequential' => 1,
      'relationship_type_id' => $relationship_type_id,
      'contact_id_a' => $contact_id,
      'is_active' => 1,
      'option.limit'  => 1000,
    );
    $result = civicrm_api('Relationship', 'get', $params);
    if (isset($result['values'])) {
      foreach ($result['values'] as $value) { 
        $afdeling_id = $value['contact_id_b'];
        // Get afdeling data.
        $params = array(
          'version' => 3,
          'sequential' => 1,
          'id' => $afdeling_id,
          'return' => 'display_name,id',
        );
        $afdelingresult = civicrm_api('Contact', 'get', $params);
        if (isset($afdelingresult['values'])) {
          $afdelingen_data[$afdelingresult['values'][0]['id']] = $afdelingresult['values'][0]['display_name'];
        }
      }
    }
  }
  if (!empty($afdelingen_data)) {
    asort($afdelingen_data);
    return $afdelingen_data;
  }
  return FALSE;
}

function ammo_get_afdeling_members($afdeling_id) {
  $afdelingmembers = &drupal_static(__FUNCTION__);
  if (!isset($afdelingmembers[$afdeling_id])) {
    if ($cache = cache_get('ammo_data')) {
      $data = $cache->data;
      if (isset($data['afdelingmembers'][$afdeling_id])) {
        $afdelingmembers[$afdeling_id] = $data['afdelingmembers'][$afdeling_id];
      }
      else {
        $afdelingmembers[$afdeling_id] = ammo_get_afdeling_members_helper($afdeling_id);
        $data['afdelingmembers'][$afdeling_id] = $afdelingmembers[$afdeling_id];
        cache_set('ammo_data', $data, 'cache', time() + 86400);
      }
    }
    else {
      $afdelingmembers[$afdeling_id] = ammo_get_afdeling_members_helper($afdeling_id);
      $data['afdelingmembers'][$afdeling_id] = $afdelingmembers[$afdeling_id];
      cache_set('ammo_data', $data, 'cache', time() + 86400);
    }
  }
  return $afdelingmembers[$afdeling_id];
}

function ammo_get_afdeling_members_helper($afdeling_id) {
  civicrm_initialize();
  $afdelingmembers = array();
  $result = db_query("SELECT c.id, c.display_name FROM {civicrm_contact} c INNER JOIN {civicrm_value_geostelsel} g ON c.id = g.entity_id INNER JOIN {civicrm_membership} m ON c.id = m.contact_id WHERE (m.membership_type_id = 1 OR m.membership_type_id = 2) AND g.afdeling = :afdeling AND c.contact_type = 'Individual' AND (m.status_id = 1 OR m.status_id = 2)", array(':afdeling' => $afdeling_id));
  if ($result) {
    while ($row = $result->fetchAssoc()) {
      $members[$row['id']] = $row['display_name'];
    }
  }
  return $members;
}

function ammo_find_members($afdeling_id, $string) {
  $selected_members = array();
  if (strlen($string) > 2) {
    // Check if participant is already subscribed.
    $members = ammo_get_afdeling_members($afdeling_id);
    foreach ($members as $member_id => $member_name) {
      if (count($selected_members) > 9) {
        break;
      }
      if (strpos(strtolower($member_name), strtolower($string)) !== FALSE) {
        $selected_members[$member_id] = $member_name . ' - ' . $member_id;
      }
    }
  }
  return $selected_members;
}

function ammo_autocomplete_members($afdeling_id, $string) {
  $selected_members = ammo_find_members($afdeling_id, $string);
  drupal_json_output($selected_members);
  exit;
}

/*
 * Gets owners or backers for amendments or motions.
 *
 * @entity_id: the amendment or motion id.
 * @entity_type: either amendment or motion.
 * @relation_type: either backer or owner.
 *
 */
function ammo_get_related_contacts($entity_id, $entity_type, $relation_type) {
  $contacts = array();
  $relation_entity_type = $entity_type . 's_' . $relation_type . 's';
  // Get related contacts.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $relation_entity_type)
    ->propertyCondition($entity_type . '_id', $entity_id);

  $result = $query->execute();

  if (isset($result[$relation_entity_type])) {
    $items_ids = array_keys($result[$relation_entity_type]);
    $items = entity_load($relation_entity_type, $items_ids);
  }
  if (!empty($items)) {
    // Create list of related contact ids.
    foreach ($items as $item) {
      $contact_entity_data[$item->contact_id] = array(
        'id' => $item->id,
        'contact_id' => $item->contact_id,
      );
    }
    civicrm_initialize();
    $result = db_query("SELECT id, display_name FROM {civicrm_contact} WHERE id IN (:ids)", array(':ids' => array_keys($contact_entity_data)));
    if ($result) {
      while ($row = $result->fetchAssoc()) {
        $contacts[$row['id']] = array(
          'contact_id' => $row['id'],
          'entity_id' => $contact_entity_data[$row['id']]['id'],
          'display_name' => $row['display_name'],
        );
      }
    }
  }
  return $contacts;
}

/*
 * @item_id: either meeting_id or document_id.
 */
function ammo_get_amendments_or_motions($type, $branch_id, $item_id) {
  $data = array();
  $items = array();
  $property = ($type == 'amendment') ? 'document_id' : 'meeting_id';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $type)
    ->propertyCondition($property, $item_id)
    ->propertyCondition('branch_id', $branch_id);

  $result = $query->execute();

  if (isset($result[$type])) {
    $items_ids = array_keys($result[$type]);
    $items = entity_load($type, $items_ids);
  }
  foreach($items as $item) {
    $data[$item->id] = $item->id;
  }
  return $data;
}

function ammo_default($form_state, $field) {
  $default_value = NULL;
  if(isset($form_state['values'][$field]))
  {
    $default_value = $form_state['values'][$field];
  }
  elseif(isset($form_state['storage'][$field]))
  {
    $default_value = $form_state['storage'][$field];
  }
  return $default_value;
}

function ammo_save_entity($type, $values, $entity_id = NULL) {
  if (empty($entity_id)) {
    $entity = entity_create($type, array());
  }
  else {
    $entity = entity_load_single($type, $entity_id);
  }
  foreach ($values as $key => $value) {
    $entity->$key = $value;
  }
  $result = entity_save($type, $entity);
  return $entity;
}

function ammo_store($entity_type, $entity_id, $mapping, &$form_state) {
  foreach($mapping as $database_field => $form_field) {
    // Store values.
    if (!empty($form_state['values'][$form_field])) {
      $form_state['storage'][$form_field] = $form_state['values'][$form_field];
    }
    $fields[$database_field] = $form_state['storage'][$form_field];
  }
  // Save amendment.
  $entity = ammo_save_entity($entity_type, $fields, $entity_id);    
  if (empty($entity)) {
    drupal_set_message('Waarden zijn niet opgeslagen.' , 'error');
  }
  else {
    drupal_set_message('Waarden zijn opgeslagen.');
  }
  return $entity;
}
