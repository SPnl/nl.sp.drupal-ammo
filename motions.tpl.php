<?php if (!empty($meeting['branches'])) : ?>
  <h1 id="inhoud">Inhoud</h1>

  <ul class="navigation">
    <?php foreach ($meeting['branches'] as $branch) : ?>
      <li>
        <a href="#branch<?php print $branch['info']['contact_id']; ?>"><strong><?php print $branch['info']['branch_display_name']; ?></strong></a>
        <ul class="motions">
          <?php foreach ($branch['motions'] as $motion) : ?>
            <li>
              <a href="#motion<?php print $motion['id']; ?>">#<?php print $motion['id']; ?></a>
            </li>
          <?php endforeach; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

  <ul>
    <?php foreach ($meeting['branches'] as $branch) : ?>
      <li>
        <h2 id="branch<?php print $branch['info']['contact_id']; ?>"><?php print $branch['info']['branch_display_name']; ?></h2>
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



