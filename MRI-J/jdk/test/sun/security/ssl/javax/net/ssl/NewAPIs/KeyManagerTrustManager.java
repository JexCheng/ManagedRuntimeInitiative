/*
 * Copyright 2001-2004 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

/*
 * @test
 * @bug 4387949 4302197 4396290 4395286
 * @summary A compile test to make sure some of the new functionality
 *      is there.  It doesn't actually call anything, just compiles it.
 *
 * 4387949: Need to add Sockets and key arrays to the
 *      X509KeyManager.choose*Alias() methods
 *      chooseServerAlias method is reverted back to accept a single
 *      keytype as a parameter, please see RFE: 4501014
 * 4302197: There's no mechanism to select one key out of many in a keystore.
 * 4396290: Need a way to pass algorithm specific parameters to TM's and KM's
 * 4395286: The property for setting the default
 *      KeyManagerFactory/TrustManagerFactory algorithms needs real name
 * @author Brad Wetmore
 */

import java.net.*;
import java.security.*;
import java.security.cert.*;
import java.security.spec.*;
import javax.net.ssl.*;

public class KeyManagerTrustManager implements X509KeyManager {

    public String[] getServerAliases(String keyType, Principal[] issuers) {
        return null;
    }
    public String[] getClientAliases(String keyType, Principal[] issuers) {
        return null;
    }
    public String chooseServerAlias(String keyType, Principal[] issuers,
            Socket socket) {
        return null;
    }
    public String chooseClientAlias(String [] keyType, Principal[] issuers,
            Socket socket) {
        return null;
    }
    public PrivateKey getPrivateKey(String alias) {
        return null;
    }
    public X509Certificate[] getCertificateChain(String alias) {
        return null;
    }

    public void doit(KeyManagerFactory kmf, TrustManagerFactory tmf,
            ManagerFactoryParameters mfp) throws Exception {
        kmf.init(mfp);
        tmf.init(mfp);
    }

    public static void main(String args[]) throws Exception {
        String kmfAlg = null;
        String tmfAlg = null;

        Security.setProperty("ssl.KeyManagerFactory.algorithm", "hello");
        Security.setProperty("ssl.TrustManagerFactory.algorithm", "goodbye");

        kmfAlg = KeyManagerFactory.getDefaultAlgorithm();
        tmfAlg = TrustManagerFactory.getDefaultAlgorithm();

        if (!kmfAlg.equals("hello")) {
            throw new Exception("ssl.KeyManagerFactory.algorithm not set");
        }
        if (!tmfAlg.equals("goodbye")) {
            throw new Exception("ssl.TrustManagerFactory.algorithm not set");
        }
    }
}
