<?php
// $Id: espauth-default.inc,v0.82 2007/12/04 12:43:03 yoshis Exp $
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
//  phpESP Default Authentication Library
//  By: Romans Jasins <roma@latnet.lv>
//      James Flemer <jflemer@alum.rpi.edu>

/* {{{ proto bool survey_auth(int survey_id, string username, string password)
   Returns true if the supplied username/password is allowed
   access to the survey. */
function survey_auth($sid, $username, $password) {
	// Default to _unauthorized_
	$auth = false;

	if (isset($GLOBALS['_GET']['auth_options']) || isset($GLOBALS['_POST']['auth_options'])) {
		$GLOBALS['errmsg'] = mkerror(_('Error processing survey: Security violation.'));
		return(false);
	}
	
	$GLOBALS['auth_options'] = array();

    // make sure database is opened
//    esp_init_db();

	// Formulate the query and check whether survey requires authentication
	$sql = "SELECT realm FROM ".TABLE_SURVEY." WHERE id = '$sid' AND public = 'N'";

	// Execute the query and put results in $accres
	$accres = mysql_query( $sql );
	if(!$accres) {
		$GLOBALS['errmsg'] = mkerror(_('Unable to execute query for access.'));
		return(false);
	}

	// Get number of rows in $accres.
	if(mysql_num_rows($accres) < 1) {
		// no matching rows ... no authorization required
		mysql_free_result($accres);
		return(true);
	}

	$realm = mysql_result($accres, 0, 0);
	mysql_free_result($accres);

	// A matching row was found - the survey requires authentication.
	//
	// Till for XOOPS by Yoshi.Sakai 2007/12/03
	//
	global $xoopsUser;
	$username = $xoopsUser->uid();
	$groups = $xoopsUser->getGroups();
	//if (!empty($username) && !empty($password)) {
	if (!empty($username)) {
		// Formulate the query check whether user is authorized
		/* $sql = "SELECT a.maxlogin, a.realm, a.resume, a.navigate
			FROM ".TABLE_ACCESS." a, ".TABLE_RESPONDENT." r
			WHERE a.survey_id = '$sid' AND
				r.username = '$username' AND
				r.password = PASSWORD('$password') AND
				r.realm = a.realm AND
				r.disabled = 'N' AND
				(r.expiration = '0' OR r.expiration > NOW())";
		*/
		$sql = "SELECT a.maxlogin, a.realm, a.resume, a.navigate
			FROM ".TABLE_ACCESS." a WHERE a.survey_id = '$sid'";
		// Execute the query and put results in $usrres
		$usrres = mysql_query( $sql );
		if(!$usrres) {
			$GLOBALS['errmsg'] = mkerror(_('Unable to execute query respondents.' ));
			return(false);
		}

		if (mysql_num_rows( $usrres ) > 0) {
			// A matching row was found - the user is authorized.
			/*
			$auth = true;
			list($maxlogin, $arealm, $aresume, $anavigate) = mysql_fetch_row($usrres);
			mysql_free_result($usrres);
			$GLOBALS['auth_options'] = array('resume' => $aresume, 'navigate' => $anavigate);
			*/
			while(list($maxlogin, $arealm, $aresume, $anavigate) = mysql_fetch_row($usrres)){
				if (in_array($arealm,$groups)){
					$auth = true;
					$GLOBALS['auth_options'] = array('resume' => $aresume, 'navigate' => $anavigate);
					break;
				}
			}
			mysql_free_result($usrres);
		}
	}

	// no matching authorization ... send a 401
	if ( ! $auth ) {
		header( "WWW-Authenticate: Basic realm=\"$realm\"" );
		header( 'HTTP/1.0 401 '. mb_('Unauthorized'));
		$GLOBALS['errmsg'] = mkerror(_('Incorrect User ID or Password, or your account has been disabled/expired.'));
		return(false);
	}

	if ( $maxlogin > 0 ) {
		// see if user is over the MAX # of responses
		$sql = "SELECT COUNT(*) < '$maxlogin' FROM ".TABLE_RESPONSE." WHERE
		survey_id = '${sid}' AND
		complete = 'Y' AND
		username = '$username'";

		$numres = mysql_query( $sql );
		$auth = mysql_result($numres, 0, 0);
		mysql_free_result($numres);
	}
	if( !$auth ) {
		header( "WWW-Authenticate: Basic realm=\"$realm\"" );
		header( 'HTTP/1.0 401 '. mb_('Unauthorized'));
		$GLOBALS['errmsg'] = mkerror(_('Your account has been disabled or you have already completed this survey.'));
		return(false);
	}
	return(true);
}
/* }}} */

/* {{{ proto bool manage_auth(string username, string password)
   Returns true if the supplied username/password is allowed
   access to the management interface. This sets/clears
   access control related session variables. */
function manage_auth($username, $password) {
	// see if session is expired, or has been logged out
	if(isset($_SESSION['acl']) &&
	   isset($_SESSION['acl']['expired'])) {
		if($_SESSION['acl']['expired']-- > 0) {
			$username = ''; $password = '';
		}
	}

	// see if ACL is cached
	if (!empty($username) &&
			!empty($password) &&
			isset($_SESSION['acl']) &&
			isset($_SESSION['acl']['username']) &&
			isset($_SESSION['acl']['password']) &&
			$_SESSION['acl']['username'] == $username) {
		$sql = "SELECT PASSWORD('$password') = '". $_SESSION['acl']['password'] ."'";
		$result = mysql_query($sql);
		$auth = mysql_result($result, 0, 0);
		mysql_free_result($result);
		if($auth)
			return(true);
	}

	$auth = false; // default to unauthorized
	$_SESSION['acl'] = array();

	if (!empty($username) && !empty($password)) {
		// Formulate the query check whether user is authorized
		$sql = "SELECT * FROM ".TABLE_DESIGNER." WHERE
			username = '$username' AND
			password = PASSWORD('$password') AND
			disabled = 'N' AND
			(expiration = '0' OR expiration > NOW())";

		// Execute the query and put results in $accres
		$accres = mysql_query( $sql );
		if(!$accres) {
			header( 'HTTP/1.0 503 '. mb_('Service Unavailable'));
			echo('<html><head><title>'. mb_('Service Unavailable') .
				'</title></head><body><h1>HTTP 503 '.
				_('Service Unavailable') .'</h1>'.
				mkerror(_('Unable to load ACL.') .' ('. mysql_error() .')') .
				'</body></html>');
			return(false);
		}

		// Get number of rows in $accres.
		if (mysql_numrows( $accres ) > 0) {
			// A matching row was found - the user is authorized.
			$auth = true;
		}
	}

	// no matching authorization ... send a 401
	if ( ! $auth ) {
		header( 'WWW-Authenticate: Basic realm="'. mb_('Management Interface') .'"' );
		header( 'HTTP/1.0 401 '. mb_('Unauthorized'));
		echo("<html>\n<head><title>401 ". mb_('Unauthorized') ."</title></head>\n".
			"<body><h1>401 ". mb_('Unauthorized') ."</h1>\n".
			mkerror(_('Incorrect User ID or Password, or your account has been disabled/expired.')) .
			"</body>\n</html>\n");
		exit;
		return(false);
	}

	// All tests passed ... create ACL array,
	// and stick it in the session
	$acl = array(
		'username'  => $username,
		'superuser' => 'N',
		'home' => '/tmp'
	);
	$fields = array('pdesign', 'pstatus', 'pdata', 'pall', 'pgroup', 'puser');
	foreach($fields as $f) {
		$$f = array();
	}
	while( $arr = mysql_fetch_array($accres, MYSQL_ASSOC) ) {
		if($arr['realm'] == 'superuser')
			$acl['superuser'] = 'Y';
		foreach($fields as $f) {
			if($arr[$f] == 'Y')
				array_push($$f, $arr['realm']);
		}
	}
	mysql_free_result($accres);

	foreach($fields as $f) {
		$acl[$f] =& $$f;
	}

	$_SESSION['acl'] =& $acl;

	// if one were to want login accounting (logs) this
	// would be the ideal place to do so...

	return(true);
}
/* }}} */

/* {{{ proto void manage_logout()
   Clears the current ACL, and will cause HTTP-Auth
   to be redisplayed. This is not fool proof; common browsers
   will continue to retry cached username & password for
   HTTP-Auth. So if the browser is not closed after logout,
   it still may be possible to get back in w/o knowing a
   valid username & password. */
function manage_logout() {
	// clear existing ACL, and set the expired flag
	//session_unset();
	$acl = array('expired' => 2);
	//session_register('acl');
	$_SESSION['acl'] = array('expired' => 2);
}
/* }}} */

/* {{{ proto boolean auth_is_owner(int surveyId, string user)
   Returns true if user owns the survey. */
function auth_is_owner($sid, $user) {
	$val = false;
	$sql = "SELECT s.owner = '$user' FROM ".TABLE_SURVEY." s WHERE s.id='$sid'";
	$result = mysql_query($sql);
	if(!(list($val) = mysql_fetch_row($result)))
		$val = false;
	mysql_free_result($result);
	return $val;
}
/* }}} */

/* {{{ proto string auth_get_survey_realm(int surveyId)
   Returns the realm of the survey. */
function auth_get_survey_realm($sid) {
	$val = '';
	$sql = "SELECT s.realm FROM ".TABLE_SURVEY." s WHERE s.id='$sid'";
	$result = mysql_query($sql);
	list($val) = mysql_fetch_row($result);
	mysql_free_result($result);
	return $val;
}
/* }}} */

/* {{{ proto boolean auth_no_access(string description)
   Handle a user trying to access an unauthorised area.
   Returns true if user should be allowed to continue.
   Returns false (or exits) if access should be denied. */
function auth_no_access($description) {
	echo(mkerror('This account does not have permission' .' '. $description .'.'));
	echo("\n<br>\n");
	echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n");
	return false;
}
/* }}} */

/* {{{ proto boolean auth_change_manager_passwd(string user, string old, string new)
   Change a managers password. If old password does not match
   or if there is an error, return false. Return true if
   password changed. */
function auth_change_manager_passwd($user,$old,$new) {
	$sql = "UPDATE ".TABLE_DESIGNER." SET password=PASSWORD('$new')
		WHERE username='$user' AND password=PASSWORD('$old')";
	return(mysql_query($sql) && mysql_affected_rows() > 0);
}
/* }}} */

/* {{{ proto boolean auth_get_option(string option)
   Returns the value of the given option. Valid options include:
   { resume, navigate } */
function auth_get_option($opt) {
	return (isset($GLOBALS['auth_options'][$opt]) && $GLOBALS['auth_options'][$opt] == 'Y');
}
/* }}} */

/* {{{ proto int auth_get_rid (int survey_id, string username, int response_id)
   Returns the RID to use for this user. */
function auth_get_rid($sid, $username, $rid = 0) {
	$rid = intval($rid);
	if (isset($rid) && $rid != 0) {
		// check for valid rid
		$sql = "SELECT (r.username = '$username' && r.complete = 'N') AS valid
				FROM ".TABLE_RESPONSE." r
				WHERE r.id = '$rid' AND
					r.survey_id = '$sid'";
		$res = mysql_query($sql);
		$valid = false;
		if ($res && mysql_num_rows($res) > 0 && mysql_result($res, 0, 0))
			$valid = true;
		if ($res)
			mysql_free_result($res);
		return ($valid) ? $rid : '';
	} elseif (auth_get_option('resume')) {
		// find latest in progress rid
		$sql = "SELECT r.id FROM ".TABLE_RESPONSE." r
				WHERE r.survey_id = '$sid' AND
					r.complete = 'N' AND
					r.username = '$username'
				ORDER BY submitted DESC
				LIMIT 1";
		$res = mysql_query($sql);
		if ($res && mysql_num_rows($res) > 0)
			$rid = mysql_result($res, 0, 0);
		if ($res)
			mysql_free_result($res);
		return ($rid != 0) ? $rid : '';
	} else {
		return '';
	}
}
/* }}} */

?>
