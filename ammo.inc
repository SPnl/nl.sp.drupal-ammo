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

function ammo_states() {
  return array(
    'concept' => 'Concept',
    'submitted' => 'Ingediend',
    'accepted' => 'Overgenomen',
    //'accepted_intention' => 'Strekking overgenomen',
    //'accepted_partially' => 'Gedeeltelijk overgenomen',
    'rejected' => 'Afgewezen',
    'withdrawn' => 'Ingetrokken',
    'invalid' => 'Ongeldig',
    'blocked' => 'Verwijderd',
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
    //'accept_amended' => 'Gewijzigd overnemen',
    //'accept_intention' => 'Strekking overnemen',
    //'accept_partially' => 'Gedeeltelijk overnemen',
    'reject' => 'Afwijzen',
  );
}

function ammo_motion_advice() {
  return array(
    'accept' => 'Overnemen',
    //'accept_amended' => 'Gewijzigd overnemen',
    //'accept_intention' => 'Strekking overnemen',
    //'accept_partially' => 'Gedeeltelijk overnemen',
    'reject' => 'Afwijzen',
  );
}

function ammo_get_afdelingen($contact_id) {
  if (empty($contact_id)) {
    $contact_id = 'all';
  }
  $afdelingen = &drupal_static(__FUNCTION__);
  if (!isset($afdelingen[$contact_id])) {
    $cache = cache_get('ammo_data');
    if (!empty($cache->data)) {
      $data = $cache->data;
      if (isset($data['afdelingen'][$contact_id])) {
        $afdelingen[$contact_id] = $data['afdelingen'][$contact_id];
        return $afdelingen[$contact_id];
      }
    }
    $afdelingen[$contact_id] = ammo_get_afdelingen_helper($contact_id);
    $data['afdelingen'][$contact_id] = $afdelingen[$contact_id];
    cache_set('ammo_data', $data, 'cache', time() + 86400);
  }
  return $afdelingen[$contact_id];
}

function ammo_get_afdelingen_helper($contact_id = NULL) {

  if ($contact_id == 'all') {
    // Return all chapters.
    if ($afdelingen = ammo_get_all_afdelingen_list()) {
      return $afdelingen;
    }
  }
  else {
    // Return contact chapter.
    $afdeling_info = ammo_get_contact_afdeling_info($contact_id);
    if (!empty($afdeling_info)) {
      return $afdeling_info;
    }
  }
  // No afdeling found.
  return FALSE;
}

function ammo_get_contact($contact_id) {
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'user')
    ->propertyCondition('status', '1', '=')
    ->fieldCondition('field_contact_id', 'value', $contact_id, '=');
  $result = $query->execute();

  if (isset($result['user'])) {
    $uid = array_key_first($result['user']);
    $user = user_load($uid);
    $user_wrapped = entity_metadata_wrapper('user', $user);
    return $user_wrapped;
  }
}

function ammo_get_contact_afdeling_info($contact_id) {
  $user_wrapped = ammo_get_contact($contact_id);
  if (!empty($afd_id = $user_wrapped->field_afdeling_id->value()) && !empty($afd_title = $user_wrapped->field_afdeling->value())) {
    return [$afd_id => $afd_title];
  }
}

function ammo_get_all_afdelingen_list() {
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'afdeling')
    ->propertyCondition('status', '1', '=')
    ->fieldCondition('afd_status', 'value', 'Erkend', '=')
    ->propertyOrderBy('title', 'ASC');
  $result = $query->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $items = node_load_multiple($nids);
    if (!empty($items)) {
      foreach ($items as $item) {
        $item_wrapped = entity_metadata_wrapper('node', $item);
        $afdelingen[$item_wrapped->afd_id->value()] = $item_wrapped->title->value();
      }
      return $afdelingen;
    }
  }
}

function ammo_get_all_members() {
  $allmembers = &drupal_static(__FUNCTION__);
  if (!isset($allmembers)) {
    if ($cache = cache_get('ammo_data')) {
      $data = $cache->data;
      if (isset($data['allmembers'])) {
        $allmembers = $data['allmembers'];
      }
      else {
        $allmembers = ammo_get_all_members_helper();
        $data['allmembers'] = $allmembers;
        cache_set('ammo_data', $data, 'cache', time() + 86400);
      }
    }
    else {
      $allmembers = ammo_get_all_members_helper();
      $data['allmembers'] = $allmembers;
      cache_set('ammo_data', $data, 'cache', time() + 86400);
    }
  }
  return $allmembers;
}

function ammo_get_all_members_helper() {
  db_set_active('cividb');
  $allmembers = array();
  $result = db_query("SELECT c.id, c.display_name FROM {civicrm_contact} c INNER JOIN {civicrm_membership} m ON c.id = m.contact_id WHERE (m.membership_type_id = 1 OR m.membership_type_id = 2) AND c.contact_type = 'Individual' AND (m.status_id = 1 OR m.status_id = 2)", array());
  if ($result) {
    while ($row = $result->fetchAssoc()) {
      $members[$row['id']] = $row['display_name'];
    }
  }
  db_set_active();
  return $members;
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
  db_set_active('cividb');
  $afdelingmembers = array();
  $result = db_query("SELECT c.id, c.display_name FROM {civicrm_contact} c INNER JOIN {civicrm_value_geostelsel} g ON c.id = g.entity_id INNER JOIN {civicrm_membership} m ON c.id = m.contact_id WHERE (m.membership_type_id = 1 OR m.membership_type_id = 2) AND g.afdeling = :afdeling AND c.contact_type = 'Individual' AND (m.status_id = 1 OR m.status_id = 2)", array(':afdeling' => $afdeling_id));
  if ($result) {
    while ($row = $result->fetchAssoc()) {
      $members[$row['id']] = $row['display_name'];
    }
  }
  return $members;
}

function ammo_find_members($scope, $string) {
  $selected_members = array();
  if (strlen($string) > 2) {
    // Check if participant is already subscribed.
    $admin_access = ammo_admin_access();
    if ($scope === 'all' && $admin_access) {
      $members = ammo_get_all_members();
    }
    else {
      $members = ammo_get_afdeling_members($scope);
    }
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

function ammo_autocomplete_members($scope, $string) {
  $selected_members = ammo_find_members($scope, $string);
  drupal_json_output($selected_members);
  exit;
}

function ammo_contact_get_display_name($contact_id) {
  db_set_active('cividb');
  $result = db_query("SELECT display_name FROM {civicrm_contact} WHERE id = :id", array(':id' => $contact_id));
  if (isset($result)) {
    $row = $result->fetchAssoc();
  }
  db_set_active();
  return empty($row) ? NULL : $row['display_name'];
}

/*
 * Gets owners or backers for amendments or motions.
 *
 * @target_id: the amendment or motion id.
 * @target_type: either amendment or motion.
 * @relation_type: either backer or owner.
 * @contact_type: either member or branch.
 *
 */
function ammo_get_related_contacts($target_id, $target_type, $relation_type = NULL, $contact_type = NULL) {
  $contacts = array();
  // Get related contacts.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'contact_relation')
    ->propertyCondition('target_id', $target_id)
    ->propertyCondition('target_type', $target_type);
  // Filter on relation_type if set.
  if (!empty($relation_type)) {
    $query->propertyCondition('relation_type', $relation_type);
  }
  // Filter on contact_type if set.
  if (!empty($contact_type)) {
    $query->propertyCondition('contact_type', $contact_type);
  }

  $query->propertyOrderBy('id', 'ASC');

  $result = $query->execute();

  if (isset($result['contact_relation'])) {
    $ids = array_keys($result['contact_relation']);
    $items = entity_load('contact_relation', $ids);
  }
  if (!empty($items)) {
    foreach ($items as $item) {
      $contacts[$item->contact_id] = array(
        'id' => $item->id,
        'contact_id' => $item->contact_id,
        'contact_email' => $item->contact_email,
        'contact_type' => $item->contact_type,
        'contact_display_name' => $item->contact_display_name,
        'branch_id' => $item->branch_id,
        'branch_display_name' => $item->branch_display_name,
      );
    }
  }
  return $contacts;
}

/*
 * Checks wether member or branch is related as owner or backer to amendment or motion.
 *
 * @target_id: the amendment or motion id.
 * @target_type: either amendment or motion.
 * @relation_type: either backer or owner.
 * @contact_type: either member or branch.
 */
function ammo_is_related($target_id, $target_type, $relation_type = NULL, $contact_type, $contact_id = NULL) {
  if ($contact_id === NULL) {
    global $user;
    $user_wrapper = entity_metadata_wrapper('user', $user);
    $contact_id = $user_wrapper->field_contact_id->value();
  }
  // Get related contacts.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'contact_relation')
    ->propertyCondition('target_id', $target_id)
    ->propertyCondition('target_type', $target_type)
    ->propertyCondition('contact_id', $contact_id)
    ->propertyCondition('contact_type', $contact_type);

  if (!empty($relation_type)) {
    $query->propertyCondition('relation_type', $relation_type);
  }

  $result = $query->execute();

  if (!empty($result)) {
    $tmp = array_pop($result);
    $tmp = array_pop($tmp);
    return $tmp->id;
  }
  return FALSE;
}

/*
 * @type: amendment or motion.
 * @branch_id: the id of the local branch.
 * @item_id: either meeting_id or document_id.
 */
function ammo_get_branch_amendments_or_motions($type, $branch_id, $item_id) {
  $data = array();
  $items = array();
  $property = ($type == 'amendment') ? 'document_id' : 'meeting_id';

  $query = db_select('ammo_' . $type . 's', 't');
  $query->join('ammo_contact_relations', 'r', 't.id = r.target_id');
  $query
    ->fields('t', array('id'))
    ->fields('r', array('id', 'branch_id'))
    //->condition('t.state', 'concept', '!=')
    ->condition('r.relation_type', 'owner','=')
    ->condition('r.contact_type', 'branch','=')
    ->condition('r.target_type', $type,'=')
    ->condition('t.' . $property, $item_id,'=');
  $result= $query->execute();
  if ($result) {
    while ($row = $result->fetchObject()) {
      $data_raw[$row->id][$row->r_id] = $row->branch_id;
    }
    foreach ($data_raw as $entity_id => $owners) {
      $first_owner = array_shift($owners);
      if ($first_owner == $branch_id) {
        $data[$entity_id] = $entity_id;
      }
    }
  }
  return $data;
}

function ammo_default($form_state, $field) {
  $default_value = NULL;
  if (isset($form_state['values'][$field]))
  {
    $default_value = $form_state['values'][$field];
  }
  elseif (isset($form_state['storage'][$field]))
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
  if (!empty($values)) {
    foreach ($values as $key => $value) {
      $entity->$key = $value;
    }
  }
  $result = entity_save($type, $entity);
  return ($result !== FALSE) ? $entity : FALSE;
}

function ammo_store($entity_type, $entity_id, $mapping, $form, &$form_state) {
  global $base_url;
  $states = ammo_states();
  $advices = $entity_type == 'amendment' ? ammo_amendment_advice() : ammo_motion_advice();
  $values = $form_state['values'];
  $fields = [];

  if (!empty($entity_id)) {
    $original_entity = entity_load_single($entity_type, $entity_id);
  }
  $differences = array();
  foreach ($mapping as $database_field => $form_field) {
    if (isset($values[$form_field])) {
      // Store values.
      $form_state['storage'][$form_field] = $values[$form_field];
    }
    if (!empty($original_entity)) {
      // Existing entity, check if there are changes.
      if ($original_entity->$database_field != $form_state['storage'][$form_field]) {
        // Store all changed fields to save in database.
        $fields[$database_field] = $form_state['storage'][$form_field];
        // Check for differences.
        if (!empty($values[$form_field])) {
          $diff_data = ammo_diff_fields($form_field);
          if (!empty($diff_data)) {
            $fieldname = $diff_data['name'];
            $differences[$database_field] = array(
              'fieldname' => $fieldname,
            );
            // Store diffline.
            switch ($diff_data['type']) {
              case 'text':
                $differences[$database_field]['diff'] = ammo_diffline($original_entity->$database_field, $form_state['values'][$form_field]);
                break;

              case 'value':
                switch ($form_field) {
                  case 'state':
                    $original = $original_entity->$database_field ? $states[$original_entity->$database_field] : '';
                    $new = $states[$form_state['values'][$form_field]];
                    break;

                  case 'advice':
                    $original = $original_entity->$database_field ? $advices[$original_entity->$database_field] : '';
                    $new = $advices[$form_state['values'][$form_field]];
                    break;

                  default:
                    $original = $original_entity->$database_field ? $original_entity->$database_field : '';
                    $new = $form_state['values'][$form_field];
                    break;
                }
                $differences[$database_field]['diff'] = $original . ' → ' . $new;
                break;
            }
          }
        }
      }
    }
    // If new entity.
    else {
      // Store all fields.
      $fields[$database_field] = $form_state['storage'][$form_field];
    }
  }

  $storage = $form_state['storage'];

  // Store motion or amendment id and create short url.
  if (!empty($original_entity)) {
    switch ($entity_type) {
      case 'amendment':
        if (empty($original_entity->chapterized_id) || $storage['chapter'] != $original_entity->chapter) {
          // Set chapterized id.
          $query = 'SELECT MAX(chapterized_id) FROM {ammo_amendments} WHERE chapter = :chapter AND document_id = :document_id';
          $result = db_query($query, array(':chapter' => $storage['chapter'], ':document_id' => $storage['document']));
          $new_id = $result->fetchField() + 1;
          $fields['chapterized_id'] = $new_id;
        }
        break;

      case 'motion':
        if (empty($original_entity->motion_id)) {
          $query = 'SELECT MAX(motion_id) FROM {ammo_motions} WHERE meeting_id = :meeting';
          $result = db_query($query, array(':meeting' => $storage['meeting']));
          $new_id = $result->fetchField() + 1;
          $fields['motion_id'] = $new_id;
        }
        break;
    }

    // Create short url.
    $long_url = $base_url . '/ammo/support/add/member/' . $entity_type . '/' . $entity_id;
    $code = ammo_shurly($entity_type, $entity_id);
    shurly_shorten($long_url, $code);
  }

  // Save entity.
  $entity = ammo_save_entity($entity_type, $fields, $entity_id);
  if (empty($entity)) {
    drupal_set_message('Waarden zijn niet opgeslagen.', 'error');
  }
  if (!empty($differences) || empty($original_entity)) {
    global $language;
    // Send mails if ammo values where changed.
    if (!empty($differences)) {
      ammo_changes_update_mail($entity_type, $entity_id, $differences, $form_state['storage']['meeting']);
    }
  }
  return $entity;
}

function ammo_populate_form($type, $entity_id, &$form_state) {
  $storage = &$form_state['storage'];
  $meeting_id = ammo_get_meeting_id($type, $entity_id);
  $meeting = entity_load_single('meeting', $meeting_id);

  // check if exists.
  $entity = entity_load_single($type, $entity_id);
  if ($entity) {
    // Check access.
    $afdelingen = ammo_afdelingen_access($meeting->admin_owner);
    // Get first owner.
    $first_owner = ammo_get_first_owner($type, $entity->id);
    if (array_key_exists($first_owner['branch_id'], $afdelingen)) {
      $storage['branch_id'] = $first_owner['branch_id'];
      $storage['branch_display_name'] = $first_owner['branch_display_name'];
      $storage['meeting'] = $meeting_id;
      $storage['meeting_display_name'] = $meeting->title;
      $storage[$type] = $entity->id;

      // Get and store values from existing amendment/motion in form.
      $mapping = array(
        'amendment' => array(
          'document_id' => 'document',
          'chapter' => 'chapter',
          'page' => 'page',
          'line' => 'line',
          'supplement' => 'supplement',
          'advice' => 'advice',
          'advice_supplement' => 'advice_supplement',
          'state' => 'state',
          'state_supplement' => 'state_supplement',
          'chapterized_id' => 'chapterized_id',
          'amendment_text' => 'amendment_text',
        ),
        'motion' => array(
          'meeting_id' => 'meeting',
          'supplement' => 'supplement',
          'advice' => 'advice',
          'advice_supplement' => 'advice_supplement',
          'state' => 'state',
          'state_supplement' => 'state_supplement',
          'motion_id' => 'motion_id',
          'consideration_opening' => 'consideration_opening',
          'consideration_body' => 'consideration_text',
          'follow_up_opening' => 'follow_up_opening',
          'follow_up_body' => 'follow_up_text',
        ),
      );
      foreach ($mapping[$type] as $database_field => $form_field) {
        $storage[$form_field] = $entity->$database_field;
      }
      if ($type === 'amendment') {
        $document = entity_load_single('document', $form_state['storage']['document']);
        $storage['document_display_name'] = $document->title;
      }
      return TRUE;
    }
    else {
      drupal_set_message('Data kon niet worden geladen.', 'error');
    }
  }
  else {
      drupal_set_message('Data kon niet worden geladen.', 'error');
  }
  return FALSE;
}

function ammo_afdelingen_access($admin_owner = NULL) {
  global $user;
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $contact_id = $user_wrapper->field_contact_id->value();

  // Afdelingen filteren op gebruikers id.
  if (ammo_admin_access() && !empty($admin_owner)) {
    $afdelingen = ammo_get_afdelingen($contact_id);
    $afdelingen = array(0 => '- ' . $admin_owner) + $afdelingen;
  }
  else {
    $afdelingen = ammo_get_afdelingen($contact_id);
  }
  return $afdelingen;
}

function ammo_checked_form(&$form, &$form_state, $type) {
  $link1 = l('gecontroleerd', 'ammo/' . $type . 's', array('attributes' => array('target' => '_blank')));
  $link2 = l('overzicht', 'ammo/' . $type . 's');
  $text1 = ($type == 'amendment') ? 'vergelijkbaar wijzigingsvoorstel' : 'vergelijkbare motie';
  $text2 = ($type == 'amendment') ? 'wijzigingsvoorstellen' : 'moties';
  $text3 = ($type == 'amendment') ? 'het wijzigingsvoorstel' : 'de motie';
  $text4 = ($type == 'amendment') ? '<p>Afdelingen kunnen wijzigingsvoorstellen indienen via deze module. Individuele leden kunnen uitsluitend wijzigingsvoorstellen indienen via het landelijk secretariaat (secretariaat@sp.nl). De wijzigingsvoorstellen van individuele leden dienen te worden ondersteund door ten minste 50 leden (handtekeningenlijst met naam en lidnummer bijvoegen).</p>' : '';
  $finaltext = $text4 . '<p>Om de discussie te kunnen concentreren op hoofdpunten, is het niet handig om tal van ' . $text2 . ' met dezelfde strekking in te dienen. Daarom vragen we onze afdelingsvoorzitters om vooraf te controleren of ' . $text3 . ' dat jij in wilt dienen, al niet eerder door een andere afdeling ingediend is. In dat geval kun je ' . $text3 . ' met één druk op de knop \'mede-indienen\'. Mocht de eerste indiener ' . $text3 . ' intrekken, dan blijft ' . $text2 . ' staan omdat jij mede-indiener bent.</p>';
  $form['checked'] = array(
    '#type' => 'checkbox',
    '#title' => 'Ik heb ' . $link1 . ' dat er niet al een ' . $text1 . ' is ingediend.',
    '#default_value' => FALSE,
    '#required' => TRUE,
		'#prefix' => $finaltext,
  );
}

function ammo_select_owner_form($type, &$form, &$form_state) {
  // Get meeting.
  $meeting = entity_load_single('meeting', $form_state['storage']['meeting']);
  // Backed by local branch?
  if (
    ammo_is_related($form_state['storage'][$type], $type, 'owner', 'branch', $form_state['storage']['branch_id']) ||
    !empty($form_state['storage']['new'])
  ) {
    $default = TRUE;
  }
  else {
    $default = FALSE;
  }
  $title = 'Ingediend door ' . check_plain($form_state['storage']['branch_display_name']);
  $form['backed'] = array(
    '#type' => 'checkbox',
    '#title' => $title,
    '#description' => $description,
    '#default_value' => $default,
    '#name' => 'backed',
    '#disabled' => $form_state['storage']['branch_id'],
  );
}

/*
 * @type: amendment or motion.
 * @entity_id: document or meeting.
 */
function ammo_select_ammo_form($type, &$form, &$form_state, $entity_id) {
  $title = ($type == 'amendment') ? 'Wijzigingsvoorstel' : 'Motie';
  $entities = ammo_get_branch_amendments_or_motions($type, $form_state['storage']['branch_id'], $entity_id);
  if (!empty($entities)) $title .= ' indienen of bewerken';
  $form[$type] = array(
    '#type' => 'select',
    '#title' => $title,
    '#options' => $entities,
    '#empty_option' => 'indienen',
    '#default_value' => ammo_default($form_state, $type),
    '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
  );
}

function ammo_select_afdeling_form(&$form, &$form_state, $type, $options) {
  $text1 = ($type == 'amendment') ? 'het wijzigingsvoorstel' : 'de motie';
  $form['branch_id'] = array(
    '#type' => 'select',
    '#title' => 'Indienen namens',
    '#description' => 'Kies namens welke afdeling u ' . $text1 . ' invoert.',
    '#options' => $options,
    '#default_value' => ammo_default($form_state, 'branch_id'),
    '#required' => true,
    '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
  );
}

function ammo_select_meeting_form(&$form, &$form_state, $options = array()) {
  if (empty($options)) {
    $options = ammo_get_meetings();
  }
  $form['meeting'] = array(
    '#type' => 'select',
    '#title' => 'Bijeenkomst',
    '#description' => 'Kies de bijeenkomst waar het wijzigingsvoorstel of de motie behandeld wordt.',
    '#options' => $options,
    '#default_value' => ammo_default($form_state, 'meeting'),
    '#required' => true,
    '#attributes' => array('onchange' => "jQuery('#action_button').mousedown()"),
  );
}

function ammo_view_entity_form($type, $entity_id) {
  $entity = entity_load_single($type, $entity_id);
  // check if exists.
  if ($entity) {
    $markup = theme($type, array('entity_id' => $entity_id));
    $form['view'] = array(
      '#type' => 'item',
      '#markup' => $markup,
    );
    return $form;
  }
  drupal_set_message('Data kon niet worden opgehaald.', 'error');
  return array();
}

function ammo_delete_entity_form($type, $entity_id) {
  $entity = entity_load_single($type, $entity_id);
  // check if exists.
  if ($entity) {
    $markup = theme($type, array('entity_id' => $entity_id, 'no_links' => TRUE));
    $form['view'] = array(
      '#type' => 'item',
      '#markup' => $markup,
    );

    $form['mail_text'] = array(
      '#type' => 'textarea',
      '#title' => 'Uitleg',
      '#required' => TRUE,
    );

    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => 'Verwijder',
      '#name' => 'delete_amot',
      '#id' => 'delete_amot',
    );
    return $form;
  }
  drupal_set_message('Data kon niet worden opgehaald.', 'error');
  return array();
}

function ammo_support_access($meeting_id) {
  // Check if meeting submit date has passed.
  if (!ammo_passed_date($meeting_id, 'event')) {
    return TRUE;
  }
  return FALSE;
}

function ammo_edit_access($meeting_id, $branch_id = NULL) {
  if (ammo_admin_access()) {
    return TRUE;
  }
  $result = ammo_access($meeting_id, 'submit', $branch_id);
  if ($result) {
    return TRUE;
  }
  return FALSE;
}

function ammo_withdraw_access($meeting_id, $branch_id = NULL) {
  return ammo_access($meeting_id, 'withdraw', $branch_id);
}

/*
 * If branch id is set checks wether user owns branch and wether the
 * date has not yet passed. If branch is not set only the latter is
 * checked.
 *
 * @branch_id: contact id local branch.
 * @meeting_id: id of meeting.
 * @date_type: submit, withdraw or meeting date.
 */
function ammo_access($meeting_id, $date_type, $branch_id = NULL) {
  // Check if meeting submit date has passed.
  if (!ammo_passed_date($meeting_id, $date_type)) {
    if ($branch_id !== NULL) {
      // Check if branch is set.
      $meeting = entity_load_single('meeting', $meeting_id);
      $afdelingen = ammo_afdelingen_access($meeting->admin_owner);
      // Check if owns afdeling.
      if (is_array($afdelingen)) {
        if (array_key_exists($branch_id, $afdelingen)) {
          return TRUE;
        }
      }
    }
    else {
      return TRUE;
    }
  }
  return FALSE;
}

function ammo_ajax_redirect($destination = NULL) {
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  $commands[] = ctools_ajax_command_redirect($destination);
  print ajax_render($commands);
  exit;
}

function ammo_set_destination(&$form_state) {
  $destination = drupal_get_destination();
  if (!isset($form_state['storage']['destination'])) {
    $form_state['storage']['destination'] = $destination;
  }
}

function ammo_get_destination() {
  $destination = drupal_get_destination();
  if ($destination['destination'] !== 'system/ajax') {
    return $destination;
  }
}

function ammo_save_relation($relation_type, $target_type, $target_id, $contact_id, $contact_type, $contact_display_name, $branch_id = NULL, $branch_display_name = NULL) {
  $contact_display_name = ammo_cleanup_branch_name($contact_display_name);
  $branch_display_name = ammo_cleanup_branch_name($branch_display_name);
  // Get contact email.
  if ($contact_id == 0) {
    $contact_email = $meeting->email;
  }
  else {
    switch ($contact_type) {
      case 'branch':
        $contact_email = ammo_get_branch_email($contact_id);
        break;
      case 'member':
        if ($contact_id == 0) {
          $meeting_id = ammo_get_meeting_id($target_type, $target_id);
          $contact_email = ammo_mailadress($meeting_id);
        }
        else {
          $contact_email = ammo_get_contact_email($contact_id);
        }
        break;
    }
  }
  $mapping = array(
    'target_type' => $target_type,
    'target_id' => $target_id,
    'relation_type' => $relation_type,
    'contact_id' => $contact_id,
    'contact_email' => $contact_email,
    'contact_type' => $contact_type,
    'contact_display_name' => $contact_display_name,
    'branch_id' => $branch_id,
    'branch_display_name' => $branch_display_name,
  );
  $entity = ammo_save_entity('contact_relation', $mapping);
  if (empty($entity)) {
    drupal_set_message('Relatie is niet opgeslagen.' , 'error');
  }
  return $entity;
}

function ammo_diff($from, $to)
{
  $diffValues = array();
  $diffMask = array();

  $dm = array();
  $n1 = count($from);
  $n2 = count($to);

  for ($j = -1; $j < $n2; $j++) $dm[-1][$j] = 0;
  for ($i = -1; $i < $n1; $i++) $dm[$i][-1] = 0;
  for ($i = 0; $i < $n1; $i++)
  {
    for ($j = 0; $j < $n2; $j++)
    {
      if ($from[$i] == $to[$j])
      {
        $ad = $dm[$i - 1][$j - 1];
        $dm[$i][$j] = $ad + 1;
      }
      else
      {
        $a1 = $dm[$i - 1][$j];
        $a2 = $dm[$i][$j - 1];
        $dm[$i][$j] = max($a1, $a2);
      }
    }
  }

  $i = $n1 - 1;
  $j = $n2 - 1;
  while (($i > -1) || ($j > -1))
  {
    if ($j > -1)
    {
      if ($dm[$i][$j - 1] == $dm[$i][$j])
      {
        $diffValues[] = $to[$j];
        $diffMask[] = 1;
        $j--;
        continue;
      }
    }
    if ($i > -1)
    {
      if ($dm[$i - 1][$j] == $dm[$i][$j])
      {
        $diffValues[] = $from[$i];
        $diffMask[] = -1;
        $i--;
        continue;
      }
    }
      {
        $diffValues[] = $from[$i];
        $diffMask[] = 0;
        $i--;
        $j--;
      }
  }

  $diffValues = array_reverse($diffValues);
  $diffMask = array_reverse($diffMask);

  return array('values' => $diffValues, 'mask' => $diffMask);
}

function ammo_diffline($line1, $line2)
{
  $diff = ammo_diff(str_split($line1), str_split($line2));
  $diffval = $diff['values'];
  $diffmask = $diff['mask'];

  $n = count($diffval);
  $pmc = 0;
  $result = '';
  for ($i = 0; $i < $n; $i++)
  {
    $mc = $diffmask[$i];
    if ($mc != $pmc)
    {
      switch ($pmc)
      {
      case -1: $result .= '</del>'; break;
      case 1: $result .= '</ins>'; break;
      }
      switch ($mc)
      {
      case -1: $result .= '<del>'; break;
      case 1: $result .= '<ins>'; break;
      }
    }
    $result .= $diffval[$i];

    $pmc = $mc;
  }
  switch ($pmc)
  {
  case -1: $result .= '</del>'; break;
  case 1: $result .= '</ins>'; break;
  }

  return $result;
}

function ammo_changes_update_mail($entity_type, $entity_id, $differences, $meeting_id) {
  global $language;
  $entity = entity_load_single($entity_type, $entity_id);
  $meeting = entity_load_single('meeting', $meeting_id);
  // Check if advice and state changes are hidden.
  $differences_hidden = $differences;
  if ($meeting->hide_state) {
    unset($differences_hidden['state']);
    unset($differences_hidden['state_supplement']);
  }
  if ($meeting->hide_advice) {
    unset($differences_hidden['advice']);
    unset($differences_hidden['advice_supplement']);
  }
  $id = ($entity_type == 'amendment') ? $entity->chapter.'.'.$entity->chapterized_id : $entity->motion_id;
  $sender_email = ammo_mailadress($meeting_id);
  if (ammo_admin_access() && !empty($meeting->admin_owner)) {
    $first_owner = $meeting->admin_owner;
  }
  else {
    $first_owner_data = ammo_get_first_owner($entity_type, $entity_id);
    $first_owner = $first_owner_data['contact_display_name'];
  }
  // Get recipients.
  $relations = ammo_get_related_contacts($entity_id, $entity_type);
  $params = array(
    'reply_to' => $sender_email,
    'format' => SWIFTMAILER_FORMAT_HTML,
    'differences' => $differences,
    'meeting_id' => $meeting_id,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
    'first_owner' => $first_owner,
    'id' => $id,
    'markup' => theme($entity_type, array('entity_id' => $entity_id, 'no_links' => TRUE)),
  );
  if ($entity_type === 'amendment') {
    $document = entity_load_single('document', $entity->document_id);
    $params['document_title'] = $document->title;
    $params['chapterized_id'] = $entity->chapterized_id;
    $params['chapter'] = $entity->chapter;
  }
  drupal_mail('ammo', 'change_message', $sender_email, $language, $params, $sender_email, TRUE);
  if (!empty($differences_hidden)) {
    drupal_set_message('Er is wordt een e-mail met de wijzigingen verzonden.');
    $params['differences'] = $differences_hidden;
    foreach ($relations as $relation) {
      if (!empty($relation['contact_email'])) {
        drupal_mail('ammo', 'change_message', $relation['contact_email'], $language, $params, $sender_email, TRUE);
      }
    }
  }
}

function ammo_withdraw_mail($entity_type, $entity_id, $owner, $meeting_id) {
  global $language;
  $entity = entity_load_single($entity_type, $entity_id);
  $id = ($entity_type == 'amendment') ? $entity->chapter.'.'.$entity->chapterized_id : $entity->motion_id;
  $sender_email = ammo_mailadress($meeting_id);
  // Get recipients.
  $relations = ammo_get_related_contacts($entity_id, $entity_type);
  $rest_owners = ammo_get_related_contacts($entity_id, $entity_type, 'owner', 'branch');
  $rest = (empty($rest_owners)) ? FALSE : TRUE;
  $params = array(
    'reply_to' => $sender_email,
    'format' => SWIFTMAILER_FORMAT_HTML,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
    'id' => $id,
    'markup' => theme($entity_type, array('entity_id' => $entity_id, 'no_links' => TRUE)),
    'meeting_id' => $meeting_id,
    'branch_display_name' => $owner->branch_display_name,
    'rest' => $rest,
  );
  if ($entity_type == 'amendment') {
    $document = entity_load_single('document', $entity->document_id);
    $params['chapter'] = $entity->chapter;
    $params['chapterized_id'] = $entity->chapterized_id;
    $params['document_title'] = $document->title;
    $params['markup'] = '<h3>' . $document->title . '</h3>' . $params['markup'];
  }
  drupal_mail('ammo', 'withdraw_ownership', $sender_email, $language, $params, $sender_email, TRUE);
  if (!empty($owner->contact_email)) {
    drupal_mail('ammo', 'withdraw_ownership', $owner->contact_email, $language, $params, $sender_email, TRUE);
  }
  foreach ($relations as $relation) {
    if (!empty($relation['contact_email'])) {
      drupal_mail('ammo', 'withdraw_ownership', $relation['contact_email'], $language, $params, $sender_email, TRUE);
    }
  }
}

function ammo_support_mail($entity_type, $entity_id, $owner, $meeting_id) {
  global $language;
  $entity = entity_load_single($entity_type, $entity_id);
  $document = entity_load_single('document', $entity->document_id);
  $id = ($entity_type == 'amendment') ? $entity->chapter.'.'.$entity->chapterized_id : $entity->motion_id;
  $sender_email = ammo_mailadress($meeting_id);
  // Get recipients.
  $relations = ammo_get_related_contacts($entity_id, $entity_type);
  $rest_owners = ammo_get_related_contacts($entity_id, $entity_type, 'owner', 'branch');
  $rest = (empty($rest_owners)) ? FALSE : TRUE;
  $params = array(
    'reply_to' => $sender_email,
    'format' => SWIFTMAILER_FORMAT_HTML,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
    'id' => $id,
    'markup' => '<h3>' . $document->title . '</h3>' . theme($entity_type, array('entity_id' => $entity_id, 'no_links' => TRUE)),
    'chapter' => $entity->chapter,
    'chapterized_id' => $entity->chapterized_id,
    'meeting_id' => $meeting_id,
    'branch_display_name' => $owner->branch_display_name,
    'rest' => $rest,
    'document_title' => $document->title,
  );
  drupal_mail('ammo', 'support', $sender_email, $language, $params, $sender_email, TRUE);
  foreach ($relations as $relation) {
    if (!empty($relation['contact_email'])) {
      drupal_mail('ammo', 'support', $relation['contact_email'], $language, $params, $sender_email, TRUE);
    }
  }
}

function ammo_delete_mail($entity_type, $entity_id, $meeting_id, $mail_text) {
  watchdog('test', $entity_type . '<br/>' . $entity_id . '<br/>' . $mail_text);
  $entity = entity_load_single($entity_type, $entity_id);
  $document = entity_load_single('document', $entity->document_id);
  $id = ($entity_type == 'amendment') ? $entity->chapter.'.'.$entity->chapterized_id : $entity->motion_id;
  $sender_email = ammo_mailadress($meeting_id);
  // Get recipients.
  $relations = ammo_get_related_contacts($entity_id, $entity_type);
  $params = array(
    'reply_to' => $sender_email,
    'format' => SWIFTMAILER_FORMAT_HTML,
    'chapter' => $entity->chapter,
    'chapterized_id' => $entity->chapterized_id,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
    'id' => $id,
    'markup' => '<h3>' . $document->title . '</h3>' . theme($entity_type, array('entity_id' => $entity_id, 'no_links' => TRUE)),
    'mail_text' => '<p>' . check_plain($mail_text) . '</p>',
    'document_title' => $document->title,
  );
  drupal_mail('ammo', 'delete_message', $sender_email, $language, $params, $sender_email, TRUE);
  foreach ($relations as $relation) {
    if (!empty($relation['contact_email'])) {
      drupal_mail('ammo', 'delete_message', $relation['contact_email'], $language, $params, $sender_email, TRUE);
    }
  }
}

function ammo_get_contact_email($contact_id) {
  $user_wrapped = ammo_get_contact($contact_id);
  if ($email = $user_wrapped->field_email_primary->value()) {
    return $email;
  }
}

function ammo_get_branch_email($contact_id) {
  $branch_wrapped = ammo_get_branch($contact_id);
  $test = $branch_wrapped->afd_email->value();
  if ($email = $branch_wrapped->afd_email->value()) {
    return $email;
  }
}

function ammo_get_branch($contact_id) {
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'afdeling')
    ->propertyCondition('status', '1', '=')
    ->fieldCondition('afd_status', 'value', 'Erkend', '=')
    ->fieldCondition('afd_id', 'value', $contact_id, '=');
  $result = $query->execute();

  if (isset($result['node'])) {
    $nid = array_key_first($result['node']);
    $chapter = node_load($nid);
    $chapter_wrapped = entity_metadata_wrapper('node', $chapter);
    return $chapter_wrapped;
  }
}

function ammo_diff_fields($source) {
  $translation = array(
    'chapter' => array(
      'name' => 'Hoofdstuk',
      'type' => 'value',
    ),
    'page' => array(
      'name' => 'Pagina',
      'type' => 'value',
    ),
    'line' => array(
      'name' => 'Regel',
      'type' => 'value',
    ),
    'consideration_opening' => array(
      'name' => 'Openingsregel overweging',
      'type' => 'text',
    ),
    'consideration_text' => array(
      'name' => 'Inhoud overweging',
      'type' => 'text',
    ),
    'follow_up_opening' => array(
      'name' => 'Openingsregel vervolg',
      'type' => 'text',
    ),
    'follow_up_text' => array(
      'name' => 'Inhoud vervolg',
      'type' => 'text',
    ),
    'supplement' => array(
      'name' => 'Toelichting',
      'type' => 'text',
    ),
    'amendment_text' => array(
      'name' => 'Wijzigingsvoorstel tekst',
      'type' => 'text',
    ),
    'advice' => array(
      'name' => 'Advies',
      'type' => 'value',
    ),
    'advice_supplement' => array(
      'name' => 'Toelichting op advies',
      'type' => 'text',
    ),
    'state' => array(
      'name' => 'Status',
      'type' => 'value',
    ),
    'state_supplement' => array(
      'name' => 'Toelichting op statuswijziging',
      'type' => 'text',
    ),
  );
  return (isset($translation[$source])) ? $translation[$source] : FALSE;
}

function ammo_get_first_owner($entity_type, $entity_id) {
  $owners = ammo_get_related_contacts($entity_id, $entity_type, 'owner', 'member');
  if (empty($owners)) {
    $owners = ammo_get_related_contacts($entity_id, $entity_type, 'owner', 'branch');
  }
  $first_owner = array_shift($owners);
  if (!empty($first_owner)) {
    return $first_owner;
  }
  return FALSE;
}

function ammo_passed_date($meeting_id, $date_type) {
  switch ($date_type) {
  case 'event':
    $field = 'date';
    break;
  case 'submit':
    $field = 'submission_closing_date';
    break;
  case 'withdraw':
    $field = 'withdrawal_closing_date';
    break;
  }
  $meeting = entity_load_single('meeting', $meeting_id);
  if (!empty($meeting->$field)) {
    $now = new DateTime();
    $last_date = new DateTime($meeting->$field);
    if ($now < $last_date) {
      return FALSE;
    }
  }
  return TRUE;
}

/*
 * @entity_type: amendment or motion.
 * @entity_id: amendment or motion id.
 */
function ammo_get_meeting_id($entity_type, $entity_id) {
  // Get meeting_id to check dates.
  $entity = entity_load_single($entity_type, $entity_id);
  if ($entity_type == 'amendment' && !empty($entity->document_id)) {
    $entity = entity_load_single('document', $entity->document_id);
  }
  if (!empty($entity->meeting_id)) {
    return $entity->meeting_id;
  }
  return FALSE;
}

function ammo_mailadress($meeting_id) {
  $meeting = entity_load_single('meeting', $meeting_id);
  $sender_mail = (!empty($meeting->email)) ? $meeting->email : variable_get('site_mail', 'webmaster@sp.nl');
  return $sender_mail;
}

function ammo_get_meetings() {
  // Check if there is more than one option to choose from.
  $meetings = entity_load('meeting', FALSE, array(), TRUE);
  $options = array();
  foreach ($meetings as $meeting) {
    if (
      (
        ammo_edit_access($meeting->id) ||
        in_array(arg(1), array('amendments', 'motions'))
      ) &&
      empty($meeting->hide_meeting)
    ) {
      $options[$meeting->id] = $meeting->title;
    }
  }
  return $options;
}

function ammo_get_documents($form_state = array()) {
  $documents = entity_load('document');
  $options = array();
  foreach ($documents as $document) {
    if (empty($form_state) || $document->meeting_id == $form_state['storage']['meeting']) {
      $options[$document->id] = $document->title;
    }
  }
  return $options;
}

/*
 * Checks wether there are member owners of a motion or amendment that may
 * be removed by the branch chairman.
 *
 */
function ammo_removable_member_owners($target_type, $entity_id) {
  global $user;
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $contact_id = $user_wrapper->field_contact_id->value();

  $contacts = ammo_get_related_contacts($entity_id, $target_type, 'owner', 'member');
  // Controleer of de betreffende leden lid zijn van afdeling voorzitter.
  $afdelingen = ammo_get_afdelingen($contact_id);
  if (!empty($afdelingen) && !empty($contacts)) {
    $afdelingen_ids = array_keys($afdelingen);
    foreach ($contacts as $member) {
      if (!in_array($member['branch_id'], $afdelingen_ids) && !ammo_superadmin_access()) {
        return FALSE;
      }
    }
  }
  else {
    return FALSE;
  }
  return TRUE;
}

function ammo_shurly($type, $id) {
  $prefix = ($type === 'amendment') ? 'a' : 'm';
  $code = $prefix . base_convert($id, 10, 36);
  return $code;
}

/**
* Removes invalid XML
*
* @access public
* @param string $value
* @return string
*/
function ammo_strip_invalid_xml($value) {
  $ret = "";
  $current;
  if (empty($value))
  {
    return $ret;
  }

  $length = strlen($value);
  for ($i=0; $i < $length; $i++)
  {
    $current = ord($value[$i]);
    if (($current == 0x9) ||
    ($current == 0xA) ||
    ($current == 0xD) ||
    (($current >= 0x20) && ($current <= 0xD7FF)) ||
    (($current >= 0xE000) && ($current <= 0xFFFD)) ||
    (($current >= 0x10000) && ($current <= 0x10FFFF)))
    {
      $ret .= chr($current);
    }
    else
    {
      $ret .= " ";
    }
  }
  return $ret;
}

function ammo_cleanup_branch_name($branch_name) {
  if (strpos($branch_name, 'SP ') === 0) {
    $branch_name = substr($branch_name, 3);
  }
  return $branch_name;
}
