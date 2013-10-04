1   class Node {
2        Node next = null;
3        int data;
4        public Node(int d) { data = d; }
5        void appendToTail(int d) {
6             Node end = new Node(d);
7             Node n = this;
8             while (n.next != null) { n = n.next; }
9             n.next = end;
10       }
11   }

这种封装appendtotail方式让人眼前一亮

1   Node deleteNode(Node head, int d) {
2        Node n = head;
3        if (n.data == d) {
4             return head.next; /* moved head */
5        }
6        while (n.next != null) {
7             if (n.next.data == d) {
8                  n.next = n.next.next;
9                  return head; /* head didn’t change */
10            }
11            n = n.next;
12       }
13   }

没有判断head是不是NULL，会出core

