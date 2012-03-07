#include <server/transaction.h>
#include <lib/request_parser.h>
#include <stdio.h>
#include <server/db.h>

typedef enum _server_state {
  INIT,
  VERIFIED,
  IDLE,
} ServerState;

static ServerState state;

static void login(Request * request);
static void register_u(Request * request);

int start_transaction() {

  state = INIT;
  connect_db();
  while (true) {
    Request * request = get_request();
    switch(request->cmd) {
      case LOGIN:
        login(request);
        fprintf(stderr, "LOGIN\n");
        break;
      case REGISTER:
        register_u(request);
        break;
      case QUIT:
        fprintf(stderr, "QUIT\n");
        state = IDLE;
        close_db();
        return 0;
    }
  }
  return 0;
}

/*
 *  the transaction methods
 */

static void login(Request * request) {
  bool result = check_user(request->params[0], request->params[1]);
  ResponseStatus rs;
  if (result) {
    rs = SUCCESS;
    state = VERIFIED; 
  } else {
    rs = FAILED;
  }
  int n = fileno(writeport);
  fprintf(stdout, "%d\n", n);
  send_response(rs, 0, NULL);
}

static void register_u(Request * request) {
  bool result = register_user(request->params[0], request->params[1]);
  ResponseStatus rs;
  if (result) {
    rs = SUCCESS;
    state = VERIFIED; 
  } else {
    rs = FAILED;
  }
  send_response(rs, 0, NULL);
}
