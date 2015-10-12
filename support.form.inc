<?php

function ammo_support_form($form, &$form_state, $type = NULL, $entity_id = NULL) {
  $form['item'] = array(
    '#type' => 'item',
    '#markup' => theme($type, array('entity_id' => $entity_id)),
  );
  $text = ($type == 'amendment') ? 'dit amendement' : 'deze motie';
  $form['support'] = array(
    '#type' => 'checkbox',
    '#title' => 'Ik ondersteun ' . $text . '.',
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
  $type = arg(2);
  $entity_id = arg(3);
  if (!$form_state['values']['support']) {
    form_set_error('support', 'Zet een vinkje');
  }
  $entity = entity_load_single($type, $entity_id);
  if (FALSE !== $entity) {
    // Get backers.
    $backers =  ammo_get_related_contacts($entity->id, $type, 'backer');
    if (in_array($user->name, array_keys($backers))) {
      form_set_error('support', 'Uw steun was al opgeslagen.');
    }
  }
  else {
    form_set_error('support', 'Het was niet mogelijk de data op te halen.');
  }
}

function ammo_support_form_submit($form, &$form_state) {
  global $user;
  $type = arg(2);
  $entity_id = arg(3);
  $entity = entity_load_single($type, $entity_id);
  // Sla nieuwe owner op.
  $owner = ammo_save_entity($type . 's_backers', array(
    $type . '_id' => $entity_id,
    'contact_id' => $user->name,
  ));
  if ($owner) {
    drupal_set_message('Uw steun is opgeslagen.');
  }
  else {
    drupal_set_message('Er is een fout opgetreden.' , 'error');
  }
  $form_state['redirect'] = ammo_get_destination();
}
