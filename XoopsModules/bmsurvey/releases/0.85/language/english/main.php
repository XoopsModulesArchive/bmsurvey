<?php
// $Id$
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2005 Yoshi.Sakai @ Bluemoon inc.         //
//                       <http://www.bluemooninc.biz/>                       //
// ------------------------------------------------------------------------- //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
define('_MD_BMSURVEY_LIST_CHECKED', 'Checked');
define('_MD_BMSURVEY_LIST_TITLE', 'Title');
define('_MD_FROM_OPTION','From address (Survey address is set at preferences)');
define('_MD_FROM_OPTION_0','Survey address');
define('_MD_FROM_OPTION_1','Users address');
define('_MD_FROM_OPTION_2',"In the questionnaire");
define('_MD_FROM_DEFRES',"Default Responce ID (Fill Input Example)");
define('_MD_BMSURVEY_LIST_UNAME', 'Owner');
define('_MD_BMSURVEY_LIST_DATE', 'Date');
define('_MD_BMSURVEY_LIST_SUBTITLE', 'Sub Title');
define('_MD_BMSURVEY_THANKS_ENTRY', 'Thank You For Completing This Survey!');
define('_MD_BMSURVEY_CAN_WRITE_USER_ONLY', 'Guest cannot edit form!');
define('_MD_ASTERISK_REQUIRED', 'Questions marked with a <font color="#FF0000">*</font> are required.');
define('_MD_MISSING_REQUIRED', 'You are missing the following required questions:');
define('_MD_MAIL_TITLE', 'Response from BMSURVEY:');
define("_MD_DENYRESULT","Deny this result");
define("_MD_DENYRESULTSURE","Deny this result. Are you sure?");
define("_MD_DENYRESULTDONE","Denied this result");
define('_MD_DEFAULTRESULT','Set default input this result');
define('_MD_DEFAULTRESULTDONE','Seted to default');
define('_MD_RESPONDENT','Respondent');
define('_MD_QUESTION_OTHER','Other');
define('_MD_BMSURVEY_FORMATERR', ' is not correctly input.');
define('_MD_BMSURVEY_DIGITERR', ' is not digit.');
define('_MD_BMSURVEY_MAXOVER', ' may not accept over %u.');
define('_MD_BMSURVEY_CHECKANY', '(Check any)');
define('_MD_BMSURVEY_CHECKLIMIT', '(Check until %u)');
define('_MD_BMSURVEY_CHECKRESET', 'Reset');

define('_MD_SUBMIT_SURVEY', 'Submit Survey');
define('_MD_NEXT_PAGE', 'Next Page');

define('_MD_POP_KEY_M','Member');
define('_MD_POP_KEY_U','Usage');
define('_MD_POP_KEY_Q','Questionnaire');
define('_MD_POP_KEY_ERR','POP-Key Error');
define('_MD_POP_CMD_NEW','Resister');
define('_MD_POP_CMD_INP','Response');
define('_MD_POP_CMD_DEL','Delete');
define('_MD_POP_MNEW_ENTRY','Risiterd user name as %s.');
define('_MD_POP_MNEW_AREADY','The user name has already been registered. Please register by another name.');
define('_MD_POP_QINP_HEADER','Please make the reply mail, input between parentheses, and transmit.
Two or more parentheses that exist in one line are choice items. () are single, [] are multiple. 
Only one [] a line are the text input items. Please input the character string. 
----

');
define('_MD_POP_QINP_FAILEDLOGIN','The user-name or the ticket number is different.');
define('_MD_POP_QINP_SUCCEEDED','%s, Your answer was registered. ');
define('_MD_POP_QINP_DELETEIT','This questionnaire has already been answered.
It is possible to delete it by replying to this mail.');
define('_MD_POP_QDEL_SUCCEEDED','%s, Your answer was deleted.');
?>
