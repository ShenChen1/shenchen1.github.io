#!/bin/bash
set -x

mkdir dummyca; cd dummyca

# Create a root certificate
mkdir -p rootca/newcerts
echo 1011 > rootca/serial
touch rootca/index.txt
cp ../openssl.cfg.rootca rootca/openssl.cfg

openssl genrsa -out rootca/RootCA.key 2048
openssl req -config rootca/openssl.cfg -new -x509 -key rootca/RootCA.key -out rootca/RootCA.pem -days 10000

ls rootca/RootCA.*

# Create a signing certificate, which is based on previously created root certificate
mkdir -p sigca/newcerts
echo 2011 > sigca/serial
touch sigca/index.txt
cp ../openssl.cfg.sigca sigca/openssl.cfg

openssl genrsa -out sigca/SigCA.key 2048
openssl req -config sigca/openssl.cfg -new -key sigca/SigCA.key -out sigca/SigCA.csr

openssl ca -config rootca/openssl.cfg -keyform pem -keyfile rootca/RootCA.key -cert rootca/RootCA.pem -in sigca/SigCA.csr -out sigca/SigCA.pem

ls sigca/SigCA.*

# Now the signing certificate SigCA.pem is ready and a signature (PKCS7, deattched, in DER format) can be created:
echo abc > abc.txt
openssl smime -sign -signer sigca/SigCA.pem -inkey sigca/SigCA.key -binary -in abc.txt -outform der -out abc.txt.p7
# This is how it can be locally verified
openssl smime -verify -inform der -in abc.txt.p7 -content abc.txt -CAfile rootca/RootCA.pem
