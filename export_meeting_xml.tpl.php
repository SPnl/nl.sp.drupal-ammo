<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE ammo [
  <!ELEMENT ammo (meeting)>
  <!ELEMENT meeting (meeting_title, documents, overview_motions)>
  <!ELEMENT meeting_title (#PCDATA)>
  <!ELEMENT documents (document)*>
  <!ELEMENT document (document_title, overview_amendments)>
  <!ELEMENT document_title (#PCDATA)>
  <!ELEMENT overview_amendments (amendments_title, chapters)>
  <!ELEMENT amendments_title (#PCDATA)>
  <!ELEMENT chapters (chapter)*>
  <!ELEMENT chapter (chapter_title, amendment_list)>
  <!ELEMENT chapter_title (#PCDATA)>
  <!ELEMENT amendment_list (amendment)*>
  <!ELEMENT amendment (amendment_id_text, amendment_proposer_text, amendment_content_title, amendment_content_text, amendment_supplement_title?, amendment_supplement_text?, amendment_advice_title, amendment_advice_type, amendment_advice_motivation)>
  <!ELEMENT amendment_id_text (#PCDATA)>
  <!ELEMENT amendment_proposer_text (#PCDATA)>
  <!ELEMENT amendment_content_title (#PCDATA)>
  <!ELEMENT amendment_content_text (#PCDATA)>
  <!ELEMENT amendment_supplement_title (#PCDATA)>
  <!ELEMENT amendment_supplement_text (#PCDATA)>
  <!ELEMENT amendment_advice_title (#PCDATA)>
  <!ELEMENT amendment_advice_type (#PCDATA)>
  <!ELEMENT amendment_advice_motivation (#PCDATA)>
  <!ELEMENT overview_motions (motions_title, motion_list)>
  <!ELEMENT motions_title (#PCDATA)>
  <!ELEMENT motion_list (motion)*>
  <!ELEMENT motion (motion_id_text, motion_proposer_text, motion_consideration_opening, motion_consideration_text, motion_follow_up_opening, motion_follow_up_text, motion_advice_title, motion_advice_type, motion_advice_motivation)>
  <!ELEMENT motion_id_text (#PCDATA)>
  <!ELEMENT motion_proposer_text (#PCDATA)>
  <!ELEMENT motion_consideration_opening (#PCDATA)>
  <!ELEMENT motion_consideration_text (#PCDATA)>
  <!ELEMENT motion_follow_up_opening (#PCDATA)>
  <!ELEMENT motion_follow_up_text (#PCDATA)>
  <!ELEMENT motion_advice_title (#PCDATA)>
  <!ELEMENT motion_advice_type (#PCDATA)>
  <!ELEMENT motion_advice_motivation (#PCDATA)>
]>
<ammo>
  <meeting>
    <meeting_title><?php print $meeting['title']; ?></meeting_title>
    <documents>
      <?php foreach ($meeting['documents'] as $document) : ?>
        <document>
          <document_title><?php print $document['title']; ?></document_title>
          <overview_amendments>
            <amendments_title>Wijzigingsvoorstellen</amendments_title>
            <chapters>
              <?php foreach ($document['chapters'] as $chapter): ?>
                <chapter>
                  <chapter_title>Hoofdstuk <?php print $chapter['chapter_nr']; ?></chapter_title>
                  <amendment_list>
                    <?php foreach ($chapter['amendments'] as $amendment) : ?>
                      <amendment>
                        <amendment_id_text>
                          <?php print $amendment['chapterized_id']; ?> (hoofdstuk <?php print $amendment['chapter']; ?> pagina <?php print $amendment['page']; ?> regel <?php print $amendment['line']; ?>)
                        </amendment_id_text>
                        <?php $list = array(); ?>
                        <?php foreach ($amendment['owners_branch'] as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
                        <?php $last = array_pop($list); ?>
                        <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
                        <amendment_proposer_text>Ingediend door afdeling <?php print $owners_list; ?>.</amendment_proposer_text>
                        <amendment_content_title>Wijzigingsvoorstel tekst:</amendment_content_title>
                        <amendment_content_text><?php print $amendment['amendment_text']; ?></amendment_content_text>
                        <?php if (!empty($amendment['supplement'])) : ?>
                          <amendment_supplement_title>Toelichting:</amendment_supplement_title>
                          <amendment_supplement_text><?php print $amendment['supplement']; ?></amendment_supplement_text>
                        <?php endif; ?>
                        <?php $options = ammo_amendment_advice(); ?>
                        <amendment_advice_title>Advies:</amendment_advice_title>
                        <amendment_advice_type><?php print $options[$amendment['advice']]; ?></amendment_advice_type>
                        <amendment_advice_motivation><?php print $amendment['advice_supplement']; ?></amendment_advice_motivation>
                      </amendment>
                    <?php endforeach; ?>
                  </amendment_list>
                </chapter>
              <?php endforeach; ?>
            </chapters>
          </overview_amendments>
        </document>
      <?php endforeach; ?>
    </documents>
    <overview_motions>
      <motions_title>Moties</motions_title>
      <motion_list>
        <?php foreach ($meeting['motions'] as $motion) : ?>
          <motion>
            <motion_id_text>Nr. <?php print $motion['motion_id']; ?></motion_id_text>
            <?php $list = array(); ?>
            <?php foreach ($owners_branch as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
            <?php $last = array_pop($list); ?>
            <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
            <motion_proposer_text><?php print $owners_list; ?></motion_proposer_text>
            <motion_consideration_opening>Overwegende dat:</motion_consideration_opening>
            <motion_consideration_text><?php print $motion['consideration_body']; ?></motion_consideration_text>
            <motion_follow_up_opening>Draagt het congres het bestuur op:</motion_follow_up_opening>
            <motion_follow_up_text><?php print $motion['follow_up_body']; ?></motion_follow_up_text>
            <motion_advice_title>Advies:</motion_advice_title>
            <motion_advice_type><?php print $motion['advice']; ?></motion_advice_type>
            <motion_advice_motivation><?php print $motion['advice_supplement']; ?></motion_advice_motivation>
          </motion>
      <?php endforeach; ?>
      </motion_list>
    </overview_motions>
  </meeting>
</ammo>
