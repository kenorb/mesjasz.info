<?php
// $Id: date-vevent.tpl.php,v 1.1.2.1 2008/06/20 13:32:42 karens Exp $
/**
 * $event
 *   An array with the following information about each event:
 * 
 *   $event['uid'] - a unique id for the event (usually the url).
 *   $event['summary'] - the name of the event.
 *   $event['start'] - the formatted start date of the event.
 *   $event['end'] - the formatted end date of the event.
 *   $event['rrule'] - the RRULE of the event, if any.
 *   $event['timezone'] - the formatted timezone name of the event, if any.
 *   $event['url'] - the url for the event.
 *   $event['location'] - the name of the event location, or a vvenue location id.
 *   $event['description'] - a description of the event.
 *   $event['alarm'] - an optional array of alarm values.
 *    @see date-valarm.tpl.php.
 */
?>
BEGIN:VEVENT
UID:<?php print $event['uid'] ?> 
SUMMARY:<?php print $event['summary'] ?> 
DTSTAMP;TZID=<?php print variable_get('date_default_timezone_name', 'UTC'); ?>;VALUE=DATE-TIME:<?php print $current_date ?> 
DTSTART;<?php print $event['timezone'] ?>VALUE=DATE-TIME:<?php print $event['start'] ?> 
<?php if (!empty($event['end'])): ?>
DTEND;<?php print $event['timezone'] ?>VALUE=DATE-TIME:<?php print $event['end'] ?> 
<?php endif; ?>
<?php if (!empty($event['rrule'])) : ?>
RRULE;<?php print $event['rrule'] ?>
<?php endif; ?>
<?php if (!empty($event['url'])): ?>
URL;VALUE=URI:<?php print $event['url'] ?> 
<?php endif; ?>
<?php if (!empty($event['location'])): ?> 
LOCATION:<?php print $event['location'] ?> 
<?php endif; ?>
<?php if (!empty($event['description'])) : ?>
DESCRIPTION:<?php print $event['description'] ?>
<?php endif; ?>
<?php if (!empty($event['valarm'])): ?>
<?php print theme('date_valarm', $event['alarm']); ?>
<?php endif; ?>
END:VEVENT