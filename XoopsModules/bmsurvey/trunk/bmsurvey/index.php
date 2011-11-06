<?php
// $Id$
//  ------------------------------------------------------------------------ //
//                      BmSurvey - Bluemoon Multi-Survey                     //
//                   Copyright (c) 2005 - 2007 Bluemoon inc.                 //
//                       <http://www.bluemooninc.biz/>                       //
//              Original source by : phpESP V1.6.1 James Flemer              //
//  ------------------------------------------------------------------------ //
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
require('../../mainfile.php');
require(XOOPS_ROOT_PATH.'/header.php');
include_once('admin/phpESP.ini.php');
include_once('./class/bmsurveyUtils.php');
if($xoopsTpl){
	bmsurveyUtils::assign_message($xoopsTpl);
}
global $xoopsModule;
$xoopsTpl->assign('survey', bmsurveyUtils::get_survey_list());
$xoopsTpl->assign('version', $xoopsModule->getVar('name')."&nbsp;V".sprintf( "%2.2f" ,  $xoopsModule->getVar('version') / 100.0) );
$xoopsTpl->assign('isUser', $xoopsUser ? true : false );

$xoopsOption['template_main'] = 'bmsurvey_list.html';
include(XOOPS_ROOT_PATH.'/footer.php');
?>
