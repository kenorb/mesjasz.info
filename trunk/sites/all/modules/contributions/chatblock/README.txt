; $Id$

This is a easy to use module that allows any authorized users to chat via an Ajax updated chat (which works via Javascript or without Javascript if necessary).

You must set the base url of your site in admin/settings/chatblock before the module will work as it is required for the Ajax interface (I haven't figured out a way around this unfortunately).

You may set the refresh rate of the iframe (5 seconds is default), the number of messages to display (100 default), and whether user 1 has to actually see the chatblock.  Also, you can configure the block by role in the block configuration.

Use this script at your own risk, although I have tried some minimal security measures (by removing any HTML or PHP tags from the input from the user).

This script is licensed under the GPL.