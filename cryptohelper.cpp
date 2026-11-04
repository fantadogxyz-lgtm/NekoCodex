#include "cryptohelper.h"

CryptoHelper* CryptoHelper::instance() {
	static CryptoHelper m_instanse;
	return &m_instanse;
}

bool CryptoHelper::initialize() {
	qDebug() << "CryptoHelper initialized with OpenSSL 3.5.6";
	return true;
}

QString CryptoHelper::encryptAES256(const QString& plainText, const QString& password)
{
	QByteArray key = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

	unsigned char iv[16];
	RAND_bytes(iv, sizeof(iv));

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(key.constData()), iv);

	QByteArray plain = plainText.toUtf8();
	QByteArray cipher(plain.size() + 16, '\0');

	int outLen = 0;
	EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(cipher.data()), &outLen,
		reinterpret_cast<const unsigned char*>(plain.constData()), plain.size());

	int finalLen = 0;
	EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(cipher.data()) + outLen, &finalLen);

	cipher.resize(outLen + finalLen);
	EVP_CIPHER_CTX_free(ctx);

	QByteArray result;
	result.append(reinterpret_cast<char*>(iv), 16);
	result.append(cipher);

	return QString::fromUtf8(result.toBase64());

}

QString CryptoHelper::decryptAES256(const QString& cipherTextBase64, const QString& password)
{
	QByteArray key = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

	QByteArray combined = QByteArray::fromBase64(cipherTextBase64.toUtf8());

	unsigned char iv[16];
	memcpy(iv, combined.constData(), 16);
	QByteArray cipher = combined.mid(16);

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(key.constData()), iv);

	QByteArray plain(cipher.size(), '\0');
	int outLen = 0;
	EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plain.data()), &outLen,
		reinterpret_cast<const unsigned char*>(cipher.constData()), cipher.size());

	int finalLen = 0;
	EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plain.data()) + outLen, &finalLen);

	plain.resize(outLen + finalLen);
	EVP_CIPHER_CTX_free(ctx);

	return QString::fromUtf8(plain);
}

QString CryptoHelper::encryptAES128(const QString& plainText, const QString& password)
{
	QByteArray key = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

	unsigned char iv[16];
	RAND_bytes(iv, sizeof(iv));

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(key.constData()), iv);

	QByteArray plain = plainText.toUtf8();
	QByteArray cipher(plain.size() + 16, '\0');

	int outLen = 0;
	EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(cipher.data()), &outLen,
		reinterpret_cast<const unsigned char*>(plain.constData()), plain.size());

	int finalLen = 0;
	EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(cipher.data()) + outLen, &finalLen);

	cipher.resize(outLen + finalLen);
	EVP_CIPHER_CTX_free(ctx);

	QByteArray result;
	result.append(reinterpret_cast<char*>(iv), 16);
	result.append(cipher);

	return QString::fromUtf8(result.toBase64());
}

QString CryptoHelper::decryptAES128(const QString& cipherTextBase64, const QString& password)
{
	QByteArray key = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

	QByteArray combined = QByteArray::fromBase64(cipherTextBase64.toUtf8());

	unsigned char iv[16];
	memcpy(iv, combined.constData(), 16);
	QByteArray cipher = combined.mid(16);

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(key.constData()), iv);

	QByteArray plain(cipher.size(), '\0');
	int outLen = 0;
	EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plain.data()), &outLen,
		reinterpret_cast<const unsigned char*>(cipher.constData()), cipher.size());

	int finalLen = 0;
	EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plain.data()) + outLen, &finalLen);

	plain.resize(outLen + finalLen);
	EVP_CIPHER_CTX_free(ctx);

	return QString::fromUtf8(plain);
}

QString CryptoHelper::encryptXOR(const QString& plainText, const QString& password)
{

	QByteArray key = password.toUtf8();
	if (key.isEmpty()) return {};


	QByteArray data = plainText.toUtf8();

	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ key[i % key.size()];
	}


	return QString::fromUtf8(data.toBase64());
}

QString CryptoHelper::decryptXOR(const QString& cipherTextBase64, const QString& password)
{

	QByteArray key = password.toUtf8();
	if (key.isEmpty()) return {};


	QByteArray data = QByteArray::fromBase64(cipherTextBase64.toUtf8());

	for (int i = 0; i < data.size(); ++i) {
		data[i] = data[i] ^ key[i % key.size()];
	}

	return QString::fromUtf8(data);
}

QString CryptoHelper::toBase64(const QString& text)
{
	return QString::fromUtf8(text.toUtf8().toBase64());
}

QString CryptoHelper::fromBase64(const QString& base64)
{
	return QString::fromUtf8(QByteArray::fromBase64(base64.toUtf8()));
}



