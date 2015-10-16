<?php

function ammo_support_form($form, &$form_state, $action, $type = NULL, $entity_id = NULL) {
  if (
    !in_array($action, array('add', 'withdraw')) ||
    !in_array($type, array('amendment', 'motion')) ||
    filter_var($entity_id, FILTER_VALIDATE_INT) === FALSE
  ) {
    form_set_error('support', 'De pagina kan niet worden geladen.');
    return array();
  }
  $form['item'] = array(
    '#type' => 'item',
    '#markup' => theme($type, array('entity_id' => $entity_id)),
  );
  switch ($action) {
  case 'add':
    switch ($type) {
    case 'amendment':
      $text = 'Ik ondersteun dit amendement.';
      break;
    case 'motion':
      $text = 'Ik ondersteun deze motie.';
      break;
    }
    break;
  case 'withdraw':
    switch ($type) {
    case 'amendment':
      $text = 'Ik trek mijn steun voor dit amendement in.';
      break;
    case 'motion':
      $text = 'Ik trek mijn steun voor deze motie in.';
      break;
    }
    break;
  }

  $form['support'] = array(
    '#type' => 'checkbox',
    '#title' => $text,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Bevestig',
    '#submit' => array('ammo_support_form_submit'),
    '#validate' => array('ammo_support_form_validate'),
  );
  return $form;
}

function ammo_support_form_validate($form, &$form_state) {
  global $user;
  $action = arg(2);
  $type = arg(3);
  $entity_id = arg(4);
  if (!$form_state['values']['support']) {
    form_set_error('support', 'Zet een vinkje');
  }
  $entity = entity_load_single($type, $entity_id);
  if (FALSE !== $entity) {
    switch ($type) {
    case 'add':
      // Check if already supported.
      $backed = ammo_is_related($entity_id, $type, 'backer'); 
      if ($backed) {
        form_set_error('support', 'Uw steun was al opgeslagen.');
      }
      break;
    case 'withdraw':
      // Check.
      $backed = ammo_is_related($entity_id, $type, 'backer'); 
      if (!$backed) {
        form_set_error('support', 'Uw steun was reeds verwijderd.');
      }
      break;
    }
  }
  else {
    form_set_error('support', 'Het was niet mogelijk de data op te halen.');
  }
}

function ammo_support_form_submit($form, &$form_state) {
  global $user;
  $action = arg(2);
  $type = arg(3);
  $entity_id = arg(4);
  if ($action === 'add') {
    $entity = entity_load_single($type, $entity_id);
    $display_name = ammo_contact_get_display_name($user->name);
    // Sla nieuwe owner op.
    $owner = ammo_save_entity('contact_relation', array(
      'target_type' => $type,
      'target_id' => $entity_id,
      'relation_type' => 'backer',
      'contact_id' => $user->name,
      'contact_type' => 'member',
      'contact_display_name' => $display_name,
    ));
    if ($owner) {
      drupal_set_message('Uw steun is opgeslagen.');
    }
    else {
      drupal_set_message('Er is een fout opgetreden.' , 'error');
    }
  }
  elseif ($action === 'withdraw') {
    $remove_id = ammo_is_related($entity_id, $type, 'backer');
    $result = entity_delete('contact_relation', $remove_id);
    if ($result !== FALSE) {
      drupal_set_message('Uw steun is ingetrokken.');
    }
    else {
      drupal_set_message('Er is een fout opgetreden.' , 'error');
    }
  }
  $form_state['redirect'] = ammo_get_destination();
}
