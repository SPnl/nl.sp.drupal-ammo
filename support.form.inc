<?php

function ammo_support_form($form, &$form_state, $action, $contact_type, $target_type = NULL, $entity_id = NULL) {
  global $user;
  if (
    !in_array($action, array('add', 'withdraw')) ||
    !in_array($contact_type, array('member', 'branch')) ||
    !in_array($target_type, array('amendment', 'motion')) ||
    filter_var($entity_id, FILTER_VALIDATE_INT) === FALSE
  ) {
    form_set_error('support', 'De pagina kan niet worden geladen.');
    return array();
  }
  $form['item'] = array(
    '#type' => 'item',
    '#markup' => theme($target_type, array('entity_id' => $entity_id)),
  );

  switch ($contact_type) {
  case 'member':
    switch ($action) {
    case 'add':
      switch ($target_type) {
      case 'amendment':
        $text = 'Ik ondersteun dit amendement.';
        break;
      case 'motion':
        $text = 'Ik ondersteun deze motie.';
        break;
      }
      break;
    case 'withdraw':
      switch ($target_type) {
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
    break;
  case 'branch':
    // Create texts.
    switch ($action) {
    case 'add':
      switch ($target_type) {
      case 'amendment':
        $text = 'De geselecteerde afdelingen dienen dit amendement mede in.';
        break;
      case 'motion':
        $text = 'De geselecteerde afdelingen dienen deze motie mede in.';
        break;
      }
      break;
    case 'withdraw':
      switch ($target_type) {
      case 'amendment':
        $text = 'De geselecteerd afdelingen dienen dit amendement niet langer (mede) in.';
        break;
      case 'motion':
        $text = 'De geselecteerde afdelingen dienen deze motie niet langer (mede) in.';
        break;
      }
      break;
    }
    // Get afdelingen.
    $branches = ammo_get_afdelingen($user->name);
    // Filter branches that already support.
    foreach ($branches as $contact_id => $branch) {
      $backed = ammo_is_related($entity_id, $target_type, 'owner', $contact_type, $contact_id); 
      switch ($action) {
      case 'add':
        if (empty($backed)) {
          $options[$contact_id] = $branch;
        }
        break;
      case 'withdraw':
        if (!empty($backed)) {
          $options[$contact_id] = $branch;
        }
        break;
      }
    }
    if (empty($options)) {
      form_set_error('support', 'Er zijn geen afdelingen gevonden die aan de voorwaarden voldoen.');
      return array();
    }
    $form['branch_id'] = array(
      '#title' => $text,
      '#type' => 'checkboxes',
      '#options' => $options,
    );
    break;
  }

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
  $values = $form_state['values'];
  $action = arg(2);
  $contact_type = arg(3);
  $target_type = arg(4);
  $entity_id = arg(5);
  $entity = entity_load_single($target_type, $entity_id);
  switch ($contact_type) {
  case 'member':
    if (!$form_state['values']['support']) {
      form_set_error('support', 'Zet een vinkje');
    }
    if (FALSE !== $entity) {
      $backed = ammo_is_related($entity_id, $target_type, 'backer', $contact_type); 
      switch ($action) {
      case 'add':
        if ($backed) {
          form_set_error('support', 'Uw steun was al opgeslagen.');
        }
        break;
      case 'withdraw':
        if (!$backed) {
          form_set_error('support', 'Uw steun was reeds verwijderd.');
        }
        break;
      }
    }
    else {
      form_set_error('support', 'Het was niet mogelijk de data op te halen.');
    }
    break;
  case 'branch':
    $selected = FALSE;
    foreach ($values['branch_id'] as $key => $value) {
      if ($key == $value) {
        $selected = TRUE;
        $backed = ammo_is_related($entity_id, $target_type, 'owner', $contact_type, $value);
        switch ($action) {
        case 'add':
          if ($backed) {
            form_set_error('support', 'Uw steun was al opgeslagen.');
          }
          break;
        case 'withdraw':
          if (!$backed) {
            form_set_error('support', 'Uw steun was reeds verwijderd.');
          }
          break;
        }
      }
    }
    if (!$selected) {
      form_set_error('contact_id', 'Vink minimaal één afdeling aan.');
    }
    break;
  }
}

function ammo_support_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $action = arg(2);
  $contact_type = arg(3);
  $target_type = arg(4);
  $entity_id = arg(5);

  switch ($contact_type) {
  case 'member':
    switch ($action) {
    case 'add':
      $entity = entity_load_single($target_type, $entity_id);
      $display_name = ammo_contact_get_display_name($user->name);
      // Sla nieuwe owner op.
      $owner = ammo_save_entity('contact_relation', array(
        'target_type' => $target_type,
        'target_id' => $entity_id,
        'relation_type' => 'backer',
        'contact_id' => $user->name,
        'contact_type' => $contact_type,
        'contact_display_name' => $display_name,
        'branch_id' => '-',
        'branch_display_name' => '-',
      ));
      if ($owner) {
        drupal_set_message('Uw steun is opgeslagen.');
      }
      else {
        drupal_set_message('Er is een fout opgetreden.' , 'error');
      }
      break;
    case 'withdraw':
      $remove_id = ammo_is_related($entity_id, $target_type, 'backer', $contact_type);
      $result = entity_delete('contact_relation', $remove_id);
      if ($result !== FALSE) {
        drupal_set_message('Uw steun is ingetrokken.');
      }
      else {
        drupal_set_message('Er is een fout opgetreden.' , 'error');
      }
      break;
    }
    break;
  case 'branch':
    switch ($action) {
    case 'add':
      foreach ($values['branch_id'] as $key => $value) {
        if ($key == $value) {
          $entity = entity_load_single($target_type, $entity_id);
          $display_name = ammo_contact_get_display_name($value);
          $display_name = substr($display_name, 12);
          // Sla nieuwe owner op.
          $owner = ammo_save_entity('contact_relation', array(
            'target_type' => $target_type,
            'target_id' => $entity_id,
            'relation_type' => 'owner',
            'contact_id' => $value,
            'contact_type' => $contact_type,
            'contact_display_name' => $display_name,
            'branch_id' => $value,
            'branch_display_name' => $display_name,
          ));
          if ($owner) {
            // Remove member owners if they exist.
            /*
            $member_owners =  ammo_get_related_contacts($entity_id, $target_type, 'owner', 'member');
            foreach ($member_owners as $key => $owner) {
              $result = entity_delete('contact_relation', $owner['id']);
              if (FALSE === $result) {
                drupal_set_message('Verwijderen contact is mislukt.', 'error');
              }
            }
             */
            drupal_set_message('Uw afdeling is (mede) indiener.');
          }
          else {
            drupal_set_message('Er is een fout opgetreden.' , 'error');
          }
        }
      }
      break;
    case 'withdraw':
      foreach ($values['branch_id'] as $key => $value) {
        if ($key == $value) {
          $remove_id = ammo_is_related($entity_id, $target_type, 'owner', $contact_type, $value);
          $result = entity_delete('contact_relation', $remove_id);
          if ($result !== FALSE) {
            drupal_set_message('U bent niet langer (mede) indiener.');
          }
          else {
            drupal_set_message('Er is een fout opgetreden.' , 'error');
          }
        }
      }
      break;
    }
    break;
  }
  $form_state['redirect'] = ammo_get_destination();
}
