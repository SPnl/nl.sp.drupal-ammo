<xml>
<?php foreach ($meeting['documents'] as $document) : ?>
<wijzigingen>
<?php foreach ($document['chapters'] as $chapter): ?>
<?php foreach ($chapter['amendments'] as $amendment) : ?>
<vote>Nr. <?php print $chapter['chapter_nr'] . '.' . $amendment['chapterized_id']; ?>: <?php $options = ammo_amendment_advice(); ?><?php print $options[$amendment['advice']]; ?></vote>
<?php endforeach; ?>
<?php endforeach; ?>
<?php endforeach; ?>
</wijzigingen>
<moties>
<?php foreach ($meeting['motions'] as $motion) : ?>
<vote>Nr. <?php print $motion['motion_id']; ?>: <?php $options = ammo_motion_advice(); ?><?php print $options[$motion['advice']]; ?></vote>
<?php endforeach; ?>
</moties>
</xml>
