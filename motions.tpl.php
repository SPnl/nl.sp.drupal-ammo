<h1 id="inhoud">Overzicht alle ingediende moties</h1>

<?php if ($meeting['admin_access']) : ?>
  <?php $states = ammo_states(); ?>
  <?php foreach ($meeting['motion_totals'] as $key => $value) : ?>
    <?php $rows[] = array($states[$key], $value); ?>
  <?php endforeach; ?>
  <?php print theme('table', array('rows' => $rows)); ?>
<?php endif; ?>

<?php if (!empty($meeting['branches']) && empty($meeting['hide'])) : ?>
  <h2 id="inhoud">Inhoud</h2>

  <ul class="navigation">
    <?php foreach ($meeting['branch_index'] as $branch_id => $branch_name) : ?>
		  <?php $branch = $meeting['branches'][$branch_id]; ?>
      <li>
        <a href="#branch<?php print $branch['info']['contact_id']; ?>"><strong><?php print ucfirst($branch['info']['branch_display_name']); ?></strong></a>
        <ul class="motions">
          <?php foreach ($branch['motions'] as $motion) : ?>
            <li>
              <a href="#motion<?php print $motion['id']; ?>"><?php print (!empty($motion['motion_id'])) ? $motion['motion_id'] : $motion_id; ?></a>
            </li>
          <?php endforeach; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

  <ul>
    <?php foreach ($meeting['branch_index'] as $branch_id => $branch_name) : ?>
		  <?php $branch = $meeting['branches'][$branch_id]; ?>
      <li>
        <h2 id="branch<?php print $branch['info']['contact_id']; ?>"><?php print ucfirst($branch['info']['branch_display_name']); ?></h2>
        <ul>
          <?php foreach ($branch['motions'] as $motion) : ?>
            <li class="ammo-element <?php print $motion['state']; ?>">
              <?php print theme('motion', array('entity_id' => $motion['id'], 'destination' => $destination)); ?>
            </li>
          <?php endforeach; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

<?php endif; ?>
