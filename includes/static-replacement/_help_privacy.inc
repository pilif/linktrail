<?php
 global $auth, $sess;

 $prp =  ($auth->auth['uid'] != "nobody") ? ('<a href="'.$sess->url(build_good_url('/Experts/'.$auth->auth['uname'].'/Settings')).'">settings</a>') : 'settings';
 $mp  =  ($auth->auth['uid'] != "nobody") ? ('<a href="'.$sess->url(build_good_url('/Experts/'.$auth->auth['uname'])).'">Mypage</a>') : 'Mypage';
 $pre =  ($auth->auth['uid'] != "nobody") ? ('<a href="'.$sess->url(build_good_url('/Experts/'.$auth->auth['uname'].'/Settings')).'">settings</a>') : 'settings';

 $tpl->set_var("MYPAGE", $mp);
 $tpl->set_var("PERSONAL_PREFERENCES", $pre);
 $tpl->set_var("PERSONAL_PROFILE", $prp);
?>