use strict;
use warnings;
use Email::Send::SMTP::Gmail;
my $mail=Email::Send::SMTP::Gmail->new( -smtp=>'smtp.gmail.com',
					-login=>'sender@gmail.com',
					-pass=>'password_goes_here');

$mail->send(-to=>'receiver@gmail.com', 
	-subject=>'SUBJECT',
	-body=>'BODY_TEXT',
	-contenttype=>'text/html');
$mail->bye;
