<?php
// $Id: date-valarm.tpl.php,v 1.1.2.1 2008/06/20 13:32:42 karens Exp $
/**
 * $alarm
 *   An array with the following information about each alarm:
 * 
 *   $alarm['action'] - the action to take, either 'DISPLAY' or 'EMAIL'
 *   $alarm['trigger'] - the time period for the trigger, like -P2D.
 *   $alarm['repeat'] - the number of times to repeat the alarm.
 *   $alarm['duration'] - the time period between repeated alarms, like P1D.
 *   $alarm['description'] - the description of the alarm.
 * 
 *   An email alarm should have two additional parts:
 *   $alarm['email'] - a comma-separated list of email recipients.
 *   $alarm['summary'] - the subject of the alarm email.
 */
?>
BEGIN:VALARM
ACTION:<?php print $alarm['action']; ?>
<?php if (!empty($alarm['trigger'])): ?>
TRIGGER:<?php print $alarm['trigger']; ?> 
<?php endif; ?>
<?php if (!empty($alarm['repeat'])): ?>
REPEAT:<?php print $alarm['repeat']; ?>
<?php endif; ?>
<?php if (!empty($alarm['duration'])): ?>
DURATION:<?php print $alarm['duration']; ?>
<?php endif; ?>
<?php if ($alarm['action'] == 'EMAIL'): ?>
ATTENDEE:MAILTO:<?php print $alarm['email'] ?>
SUMMARY:<?php print $alarm['summary'] ?>
<?php endif; ?>
DESCRIPTION:<?php print $alarm['description'] ?>
END:VALARM