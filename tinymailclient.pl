use strict;
use warnings;
use Email::Send::SMTP::Gmail;
my $mail=Email::Send::SMTP::Gmail->new( -smtp=>'smtp.gmail.com',
								-login=>'example@gmail.com',
								-pass=>'this_is_your_password');

$mail->send(-to=>'example@example.example', 
		-subject=>'SUBJECT',
		-body=>'ALARM',
		-contenttype=>'text/html');
$mail->bye;
