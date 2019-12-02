<xml>
<?php foreach ($meeting['documents'] as $document) : ?>
<document_title><?php print $document['title']; ?></document_title>
<amendments_title>Wijzigingsvoorstellen</amendments_title>
<?php foreach ($document['chapters'] as $chapter): ?>
<chapter_title>Hoofdstuk <?php print $chapter['chapter_nr']; ?></chapter_title>
<?php foreach ($chapter['amendments'] as $amendment) : ?>
<amendment_id_text>Wijzigingsvoorstel nr. <?php print $chapter['chapter_nr'] . '.' . $amendment['chapterized_id']; ?> (hoofdstuk <?php print $chapter['chapter_nr']; ?>, pagina <?php print $amendment['page']; ?>, regel <?php print $amendment['line']; ?>)</amendment_id_text>
<?php $list = array(); ?>
<?php foreach ($amendment['owners_branch'] as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
<?php $last = array_pop($list); ?>
<?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
<amendment_proposer_text>Ingediend door <?php print $owners_list; ?>.</amendment_proposer_text>
<amendment_content_title>Wijzigingsvoorstel tekst:</amendment_content_title>
<amendment_content_text><?php print $amendment['amendment_text']; ?></amendment_content_text>
<?php if (!empty($amendment['supplement'])) : ?>
<amendment_supplement_title>Toelichting:</amendment_supplement_title>
<amendment_supplement_text><?php print $amendment['supplement']; ?></amendment_supplement_text>
<?php endif; ?>
<?php $options = ammo_amendment_advice(); ?><amendment_advice_intro><amendment_advice_title>Advies:</amendment_advice_title><amendment_advice_type><?php print $options[$amendment['advice']]; ?></amendment_advice_type></amendment_advice_intro>
<amendment_advice_motivation><?php print $amendment['advice_supplement']; ?></amendment_advice_motivation>
<?php endforeach; ?>
<?php endforeach; ?>
<?php endforeach; ?>
<motions_title>Moties</motions_title>
<?php foreach ($meeting['motions'] as $motion) : ?>
<motion_id_text>Nr. <?php print $motion['motion_id']; ?></motion_id_text>
<?php $list = array(); ?>
<?php foreach ($motion['owners_branch'] as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
<?php $last = array_pop($list); ?>
<?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
<motion_proposer_text><?php print $owners_list; ?></motion_proposer_text>
<motion_consideration_opening>Overwegende dat:</motion_consideration_opening>
<motion_consideration_text><?php print $motion['consideration_body']; ?></motion_consideration_text>
<motion_follow_up_opening>Draagt het congres het bestuur op:</motion_follow_up_opening>
<motion_follow_up_text><?php print $motion['follow_up_body']; ?></motion_follow_up_text>
<?php $options = ammo_motion_advice(); ?><motion_advice_intro><motion_advice_title>Advies:</motion_advice_title><motion_advice_type><?php print $options[$motion['advice']]; ?></motion_advice_type></motion_advice_intro>
<motion_advice_motivation><?php print $motion['advice_supplement']; ?></motion_advice_motivation>
<?php endforeach; ?>
</xml>
