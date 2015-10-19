<?php dpm($variables); ?>
<h3 id="amendment<?php print $entity_id; ?>">Amendement nr. <?php print $entity_id; ?></h3>

<div class="ammo-hidden">
<?php if ($first_owner['contact_type'] == 'member') : ?>
  <?php foreach ($owners_member as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $owners_list; ?> van afdeling <?php print $first_owner['branch_display_name']; ?>.</p>
  <?php $number = count($backers); ?>
  <p>Ondersteunt door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?>.</p>
<?php else : ?>
  <?php foreach ($owners_branch as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door afdeling <?php print $owners_list; ?>.</p>
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
<p>
  <strong>Toelichting:</strong><br/>
  <?php print $supplement; ?>
</p>
<?php if (!empty($advice)) : ?>
<?php $options = ammo_amendment_advice(); ?>
  <p>
    <strong>Advies:</strong> <?php print strtolower($options[$advice]); ?>
    <?php if (!empty($advice_supplement)) : ?>
      <br/><?php print $advice_supplement; ?>
    <?php endif; ?>
  </p>
<?php endif; ?>
</div>

<?php if (arg(1) !== 'support') : ?>
  <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
  <ul class="ammo-list">
    <li><a href="#inhoud">^</a></li>
    <?php if ($edit_access) : ?>
      <li><?php print l('bewerk', 'ammo/amendment/edit/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if ($admin_access) : ?>
      <li><?php print l('bewerk advies', 'ammo/amendment/advice/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if (!$admin_access) : ?>
      <?php if (empty($owners_branch)) : ?>
        <?php if ($backed_by_user) : ?>
          <li><?php print l('trek steun als lid in', 'ammo/support/withdraw/member/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php else  : ?>
            <li><?php print l('steun als lid', 'ammo/support/add/member/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php endif; ?>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($unsupported_branches) : ?>
      <li><?php print l('mede indienen als afdeling', 'ammo/support/add/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if ($supported_branches) : ?>
      <li><?php print l('intrekken als afdeling', 'ammo/support/withdraw/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
  </ul>
<?php endif; ?>
