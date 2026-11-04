#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/rand.h>

class CryptoHelper  : public QObject
{
	Q_OBJECT

public:

	CryptoHelper(CryptoHelper& other) = delete;
	void operator=(const CryptoHelper& other) = delete;

	static  CryptoHelper* instance();
	bool initialize();


	//AES-256
	static QString encryptAES256(const QString& plainText, const QString& password);
	static QString decryptAES256(const QString& cipherTextBase64, const QString& password);

	//AES-128	
	static QString encryptAES128(const QString& plainText, const QString& password);
	static QString decryptAES128(const QString& cipherTextBase64, const QString& password);

	//XOR
	static QString encryptXOR(const QString& plainText, const QString& password);
	static QString decryptXOR(const QString& cipherTextBase64, const QString& password);

	//Base64
	static QString toBase64(const QString& text);
	static QString fromBase64(const QString& base64);

private:
	CryptoHelper() = default;
	~CryptoHelper() = default;
};

