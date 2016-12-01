<html>
<meta http-equiv="Content-Type" content="text/html">
<body>
<?php foreach ($meeting['documents'] as $document) : ?>
  <?php foreach ($document['chapters'] as $chapter) : ?>
    <?php foreach ($chapter['pages'] as $page) : ?>
      <?php foreach ($page['amendments'] as $amendment) : ?>
        <h2><?php print $document['title']; ?></h2>
        <p><strong>Wijzigingsvoorstel nr. <?php print $amendment['id']; ?>, hoofdstuk <?php print $amendment['chapter']; ?>, pagina <?php print $amendment['page']; ?>, regel <?php print $amendment['line']; ?></strong></p>
        <?php if (!empty($amendment['owners_branch'])) : ?>
          <?php $list = array(); ?>
          <?php foreach ($amendment['owners_branch'] as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
          <?php $last = array_pop($list); ?>
          <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
        <?php endif; ?>
        <?php if (!empty($amendment['owners_member'])) : ?>
          <?php $list = array(); ?>
          <?php foreach ($amendment['owners_member'] as $owner_member) : $list[] = $owner_member['contact_display_name']; endforeach; ?>
          <?php $last = array_pop($list); ?>
          <?php if (count($list) === 0) : $members_list = $last; else : $members_list = implode(', ', $list) . ' en ' . $last; endif; ?>
          <p>Ingediend door <?php print $members_list; ?> van afdeling <?php print $owner_member['branch_display_name']; ?>.</p>
          <?php $number = count($amendment['backers']) + count($amendment['owners_member']); ?>
          <?php if (!empty($amendment['owners_branch'])) : ?>
            <p>Mede ingediend door afdeling <?php print $owners_list; ?>.</p>
          <?php endif; ?>
        <p>Ondersteund door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?><?php if (empty($amendment['owners_branch'])) : print ' ('; print ($number >= 5) ? 'voldoende' : 'onvoldoende'; print ' steun)'; endif ?>.</p>
        <?php else: ?>
          <?php if (!empty($amendment['owners_branch'])) : ?>
            <p>Ingediend door afdeling <?php print $owners_list; ?>.</p>
          <?php endif; ?>
        <?php endif; ?>
        <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
        <p><strong>Wijzigingsvoorstel tekst</strong></p>
        <p><?php print $amendment['amendment_text']; ?></p>
        <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
        <?php if (!empty($amendment['supplement'])) : ?>
          <p><strong>Toelichting</strong></p>
          <p><?php print $amendment['supplement']; ?></p>
          <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
        <?php endif; ?>
        <?php $options = ammo_states(); ?>
        <p><strong>Status:</strong> <?php print $options[$amendment['state']]; ?></p>
        <?php if (!empty($amendment['advice'])) : ?>
          <?php $options = ammo_amendment_advice(); ?>
          <p><strong>Advies:</strong> <?php print $options[$amendment['advice']]; ?></p>
          <?php if (!empty($amendment['advice_supplement'])) : ?>
            <p><?php print $amendment['advice_supplement']; ?></p>
          <?php endif; ?>
          <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
        <?php endif; ?>
        <br style="page-break-before: always">
      <?php endforeach; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
<?php endforeach; ?>
</body>
</html>
