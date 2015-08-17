#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2015 Liqiang Guo. All Rights Reserved.
# Author: Liqiang Guo (xxx@gmail.com)
# Date  : 2015-08-17 03:26:27
# File  : email.py
# Brief :
# http://stackoverflow.com/questions/26852128/smtpauthenticationerror-when-sending-mail-using-gmail-and-python

import smtplib
from email.mime.text import MIMEText

def SendEmail(you, content):
  msg = MIMEText(content, _subtype='plain')
  msg['Subject'] = 'HelloWord'
  msg['From'] = 'xxx@gmail.com'
  msg['To'] = you

  # Send the message via our own SMTP server, but don't include the
  # envelope header.
  s = smtplib.SMTP('smtp.gmail.com', 587)
  s.ehlo() 
  s.starttls() 
  s.login('xxx@gmail.com', 'xxx') 
  s.sendmail('xxx@gmail.com', you, msg.as_string())
  s.quit()

if __name__ == '__main__':
  SendEmail('zzz@126.com', 'HelloWord')
