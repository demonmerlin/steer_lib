/*
  The RealityGrid Steering Library

  Copyright (c) 2002-2010, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
 */

import java.io.InputStream;
import java.io.PrintStream;
import java.net.Socket;

public class HybridThread extends Thread {

  HybridSwitch sw;
  Socket s;
  PrintStream os;
  InputStream in;
  String this_id;
  String src_id;

  public HybridThread(Socket s, HybridSwitch sw) {
    try {

      this.s = s;
      this.sw = sw;

      os = new PrintStream(s.getOutputStream());
      in = s.getInputStream();

      byte c = ' ';
      int idx = 0;
      byte buffer[] = new byte[1024];
      while (c != '\n' && idx < buffer.length) {
	c = (byte) in.read();
	if (c != '\n') {
	  buffer[idx++] = c;
	}
      }

      // The ID of this connection
      this_id = new String(buffer);
      this_id = this_id.trim();
      System.out.println("New connection [" + this_id + "]");

      int max = idx;
      idx = 0;
      while (idx < max) {
	buffer[idx++] = (byte) 0;
      }

      c = ' ';
      idx = 0;
      while (c != '\n' && idx < buffer.length) {
	c = (byte) in.read();
	if (c != '\n') {
	  buffer[idx++] = c;
	}
      }

      // The ID of the data source it's subscribing to (or "NO_DATA"
      // if we're output only)
      src_id = new String(buffer);
      src_id = src_id.trim();
      System.out.println("subscribing to [" + src_id + "]");

      sw.register_thread(src_id, this);

      start();

    } catch (Exception ex) {
      ex.printStackTrace(System.err);
    }
  }

  public void run() {
    try {
      byte buffer[] = new byte[1024];

      for (;;) {

	byte c = ' ';

	// System.out.println( "Searching for tag..." );
	while (c != '#') {
	  c = (byte) in.read();
	  if (c == -1) {
	    System.out.println("...read failed");
	    throw new Exception();
	  }
	  // System.out.println( "B " );
	}

	// System.out.println( "Reading tag..." );
	int idx = 0;
	buffer = new byte[1024];
	while (c != '\n' && idx < buffer.length) {
	  c = (byte) in.read();
	  if (c == -1) {
	    throw new Exception();
	  }
	  if (c != '\n') {
	    buffer[idx++] = c;
	  }
	}

	String to = new String(buffer);
	System.out.println("--Dest. tag: " + to);

	c = ' ';
	idx = 0;
	buffer = new byte[1024];
	while (c != '\n' && idx < buffer.length) {
	  c = (byte) in.read();
	  if (c == -1) {
	    throw new Exception();
	  }
	  if (c != '\n') {
	    buffer[idx++] = c;
	  }
	}

	String id = new String(buffer);
	System.out.println("--ID: " + id);

	c = ' ';
	idx = 0;
	buffer = new byte[1024];
	while (c != '\n' && idx < buffer.length) {
	  c = (byte) in.read();
	  if (c == -1) {
	    throw new Exception();
	  }
	  if (c != '\n') {
	    buffer[idx++] = c;
	  }
	}

	String length_s = new String(buffer);
	length_s = length_s.trim();
	// System.out.println( "--Length of data: "+length_s );

	int length = Integer.parseInt(length_s);
	int cur = 0;
	int j;

	byte[] b = new byte[length];

	while (length - cur > 0) {
	  j = in.read(b, cur, length - cur);

	  if (j == -1) {
	    throw new Exception();
	  }
	  cur += j;
	  // System.out.println( "Read "+cur +" of "+ length );
	}

	to = to.trim();
	id = id.trim();

	byte[] ackMsg = new byte[2];
	ackMsg[0] = '0';
	ackMsg[1] = '\n';

	if (to.endsWith("_REG_ACK")) {
	  if (!sw.forwardAck(to)) {
	    // Send ioProxy ack back to sender of this ack
	    // but don't actually send ack on to the data
	    // source because not all the subscribers
	    // have acknowledged yet
	    System.out
	      .println("ARPDBG: not everyone has ack'd yet...");
	    ackMsg[0] = '1';
	    os.write(ackMsg);
	    continue;
	  }
	  System.out.println("ARPDBG: sending ack through");
	}

	if (sw.send(this_id, to, id, length, b)) {
	  System.out.println("ARPDBG: sent to [" + to + "] OK");
	  ackMsg[0] = '1';
	}
	os.write(ackMsg);
      }
    } catch (Exception ex) {
      ex.printStackTrace(System.err);
      close();
    }
  }

  public synchronized void send(String from, String id, byte[] data) {
    try {
      // System.out.println("send, from: " + from);
      os.write(data);

    } catch (Exception ex) {
      ex.printStackTrace(System.err);
      close();
    }
  }

  private void close() {
    try {
      s.close();
      in.close();
    } catch (Exception ex) {
    }
    sw.deregister_thread(src_id, this);
  }
}
