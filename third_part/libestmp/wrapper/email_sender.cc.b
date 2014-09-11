/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "../include/auth-client.h"
#include "../include/libesmtp.h"

static std::string toAscii(const std::string & str) {
  std::string rs;
  for (std::string::const_iterator iter = str.begin();
    iter != str.end(); iter++) {
    unsigned int c = *iter;
    if (c > 0x7F) c = '?';
    rs.push_back(c);
  }
  return rs;
}

// The following two classes implement an C++ SMTP wrapper over libesmtp.
// The same signatures could be implemented over different SMTP implementations
// or libesmtp could be combined with libgmime to enable support for non-ASCII
// content.  

// SMTP Session.
class SMTPSession {
 public:
 // Create new instance.
  SMTPSession(const std::string & smtpHost,
              int smtpPort,
              const std::string & smtpUsername,
              const std::string & smtpPassword) :
                  session(0), authctx(0), 
                  user(smtpUsername), 
                  pwd(smtpPassword) {
     auth_client_init();
     session = smtp_create_session();
     if (session == 0) {
       LOG(ERROR) << "Could not initialize session.";
       return;
     }
     std::string host = smtpHost;
     host.append(1, ':');
     host.append(IntToString(smtpPort));
     smtp_set_server(session, host.c_str());

     authctx = auth_create_context();
     auth_set_mechanism_flags(authctx, AUTH_PLUGIN_PLAIN, 0);
     auth_set_interact_cb(authctx, authinteract, (void*) this);

     if (!user.empty() || !pwd.empty()) {
       smtp_auth_set_context(session, authctx);
     }
   }

   ~SMTPSession() {
     smtp_destroy_session(session);
     auth_destroy_context(authctx);
   }

   void send() {
     int status = smtp_start_session(session);
     if (!status) {
       LOG(ERROR) << "send failed!";
     }
   }

   operator smtp_session_t() {
     return session;
   }

 private:
   SMTPSession(SMTPSession&);
   SMTPSession& operator=(SMTPSession&);
   smtp_session_t session;
   auth_context_t authctx;
   std::string user;
   std::string pwd;

   // This method is called if the SMTP server requests authentication.
   static int authinteract(auth_client_request_t request, char **result, int fields,
       void *arg) {
     SMTPSession* pThis = (SMTPSession*) arg;
     for (int i = 0; i < fields; i++) {
       int flag = request[i].flags & 0x07;
       if (flag == AUTH_USER) {
         result[i] = const_cast<char *>(pThis->user.c_str());
       } else if(flag == AUTH_PASS) {
         result[i] = const_cast<char *>(pThis->pwd.c_str());
       }
     }
     return 1;
   }
};

// A message in an SMTP session.
class SMTPMessage {
 public:
  SMTPMessage(SMTPSession& session, 
              const std::string & from, 
              const std::string & to,
              const std::string & cc,
              const std::string & bcc,
              const std::string & subject,
              const std::string & msg) {
      message = smtp_add_message(session);
      body = current = toMessage(msg);
      smtp_set_reverse_path(message, from.c_str());
      addRecipients(to, "To");
      addRecipients(cc, "Cc");
      addRecipients(bcc, "Bcc");
      if (!subject.empty()) {
        smtp_set_header(message, "Subject", subject);
      }
      smtp_set_messagecb(message, messagecb, this); 
  }
  ~SMTPMessage() {}
 private:
  SMTPMessage(const SMTPMessage&);
  SMTPMessage& operator=(const SMTPMessage&);
  smtp_message_t message;
  std::string body;
  std::string current;

  void addRecipients(const std::string & addresses, const char * field) {
    if (!addresses.empty()) {
      char * str = const_cast<char *>(addresses.c_str());
      smtp_set_header(message, field, NULL, str);
      char* last;
      for(char* next = strtok(str, ",", &last); next;
        next = strtok(NULL, ",", &last)) {
        smtp_add_recipient(message, next);
      }
    }
  }
  // Message bodies can only contain US-ASCII characters and 
  // CR and LFs can only occur together.
  static std::string toMessage(const std::string & str) {
    // count the number of carriage returns and line feeds
    int feedCount = 0;
    for (size_t pos = str.find_first_of(std::string("\n\r"));
         pos != std::string::npos;
         pos = str.find_first_of(std::string("\n\r"), ++pos)) {
      feedCount++;
    }
    // allocate sufficient space for the modified message
    std::string rs(str.size() + feedCount + 1, 0)
    char * retval = &rs[0];
    char * current = retval;
    char * startOfLine = current;
    // iterator through message
    for (std::string::const_iterator iter = str.begin();
      iter != str.end(); iter++) {
        unsigned int c = *iter;
        // replace non-ASCII characters with '?'
        if (c > 0x7F) {
          *current++ = 0x3F; // '?'
        } else if (c == 0x0A || c == 0x0D) {
          // replace any stray CR or LF with CRLF
          // reset start of line
          *current++ = 0x0D;
          *current++ = 0x0A;
          startOfLine = current;
          std::string::const_iterator next = iter + 1;
          if (next != str.end() && (*next == 0x0A || *next == 0x0D)) {
            iter++;
          }
        } else {
          // truncate any lines to 1000 characters (including CRLF)
          // as required by RFC.
          if (current < startOfLine + 998) {
            *current++ = (char) c;
          }
        }
      }
      return rs;
    }

    // Callback for message.
    static std::string messagecb(void ** ctx, int* len, void* arg) {
      *ctx = 0;
      std::string retval;
      SMTPMessage* pThis = (SMTPMessage*) arg;
      // rewind message
      if (len == NULL) {
        pThis->current = pThis->body;
      } else {
        if (!pThis->current.empty()) {
          *len = pThis->current.size();
        }
        retval = pThis->current;
        pThis->current.clear();
      }
      return retval;
    }

};

int main(int argc, char** argv) {
  return 0;
}
