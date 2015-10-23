<h3 id="amendment<?php print $entity_id; ?>">Amendement nr. <?php print $entity_id; ?></h3>
<?php //print '<p>' . $meeting_title . ' van de SP, in vergadering bijeen op ' . $meeting_date . '.</p>'; ?>
<?php if (!empty($owners_branch)) : ?>
  <?php foreach ($owners_branch as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
<?php endif; ?>
<?php if (!empty($owners_member)) : ?>
  <?php foreach ($owners_member as $owner_member) : $list[] = $owner_member['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $members_list = $last; else : $members_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $members_list; ?> van afdeling <?php print $owner_member['branch_display_name']; ?>.</p>
  <?php $number = count($backers) + count($owners_member); ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Mede ingediend door afdeling <?php print $owners_list; ?>.</p>
  <?php endif; ?>
  <p>Ondersteunt door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?>.</p>
<?php else: ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Ingediend door afdeling <?php print $owners_list; ?>.</p>
  <?php endif; ?>
<?php endif; ?>
<?php switch ($type): ?>
<?php case 'specific': ?>
<p>Regel <?php print $line; ?></p>
<?php switch ($action): ?>
<?php case 'add': ?>
<p><strong>Toe te voegen tekst:</strong><br/><?php print $new_text; ?></p>
<?php break; ?>
<?php case 'delete': ?>
<p><strong>Te verwijderen tekst:</strong><br/><?php print $current_text; ?></p>
<?php break; ?>
<?php case 'replace': ?>
<p><strong>De tekst:</strong><br/><?php print $current_text; ?><br/><strong>vervangen door:</strong><br/><?php print $new_text; ?></p>
<?php break; ?>
<?php endswitch; ?>
<?php break; ?>
<?php case 'general': ?>
<p><strong>Tekst:</strong><br/><?php print $general; ?></p>
<?php break; ?>
<?php endswitch; ?>
<?php if (!empty($supplement)) : ?>
  <p>
    <strong>Toelichting:</strong><br/>
    <?php print $supplement; ?>
  </p>
<?php endif; ?>
<?php if (!empty($advice)) : ?>
<?php $options = ammo_amendment_advice(); ?>
  <p>
    <strong>Advies:</strong> <?php print strtolower($options[$advice]); ?>
    <?php if (!empty($advice_supplement)) : ?>
      <br/><?php print $advice_supplement; ?>
    <?php endif; ?>
  </p>
<?php endif; ?>

<?php if ($no_links !== TRUE) : ?>
  <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
  <ul class="ammo-list">
    <li><a href="#inhoud">^</a></li>
    <?php if ($edit_access) : ?>
      <li><?php print l('bewerk', 'ammo/amendment/edit/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if (($admin_access && $support_access) || $superadmin_access) : ?>
      <li><?php print l('bewerk advies', 'ammo/amendment/advice/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if (!$admin_access) : ?>
      <?php if ($support_access) : ?>
        <?php if (!empty($owners_member)) : ?>
          <?php if ($backed_by_user) : ?>
            <li><?php print l('trek steun als lid in', 'ammo/support/withdraw/member/amendment/' . $entity_id, array('query' => $dest))?></li>
          <?php else  : ?>
              <li><?php print l('steun als lid', 'ammo/support/add/member/amendment/' . $entity_id, array('query' => $dest))?></li>
          <?php endif; ?>
        <?php endif; ?>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($withdraw_access) : ?>
      <?php if ($unsupported_branches) : ?>
        <li><?php print l('mede indienen als afdeling', 'ammo/support/add/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if ($supported_branches) : ?>
        <li><?php print l('intrekken als afdeling', 'ammo/support/withdraw/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
    <?php endif; ?>
  </ul>
<?php endif; ?>
