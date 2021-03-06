import os
import shutil
import platform
import pytest
import subprocess
from mangofmt import MangoImage, EncryptionType, CompressionType

def is_root():
    # there is no os.geteuid on windows
    # just skip those tests
    if platform.system() == "Windows":
        return False
    else:
        return os.geteuid() == 0


def test_open():
    MangoImage.from_path("test.jpg")


def test_filename():
    img = MangoImage.from_path("test.jpg")
    assert img.meta_data.filename == "test.jpg"


def test_mime():
    img = MangoImage.from_path("test.jpg")
    assert img.meta_data.mime == "JPEG"


@pytest.mark.skipif(shutil.which("sha256sum") is None, reason="sha256sum cli is not available on the system")
def test_checksum():
    img = MangoImage.from_path("test.jpg")
    meta = img.meta_data
    img_sum = meta.checksum
    sys_proc = subprocess.run(["sha256sum", "test.jpg"], stdout=subprocess.PIPE)
    sys_sum = sys_proc.stdout.decode("utf-8").split(" ")[0]

    print(img_sum)
    print(sys_sum)
    assert img_sum == sys_sum


@pytest.mark.skipif(shutil.which("sha256sum") is None, reason="sha256sum cli is not available on the system")
def test_checksum_onelinner():
    """
    This test exist for ensuring that you can put everything on one line,
    something a python developer might do
    """
    img_sum = MangoImage.from_path("test.jpg").meta_data.checksum
    sys_proc = subprocess.run(["sha256sum", "test.jpg"], stdout=subprocess.PIPE)
    sys_sum = sys_proc.stdout.decode("utf-8").split(" ")[0]

    print(img_sum)
    print(sys_sum)
    assert img_sum == sys_sum


def test_encryption_none():
    img_enc = MangoImage.from_path("test.jpg").meta_data.encryption
    assert img_enc is None


@pytest.mark.skipif(not CompressionType.GZIP.is_supported(), reason="no GZIP support")
def test_compress():
    img = MangoImage.from_path("test.jpg")
    img_data = img.image_data
    img.compress(CompressionType.GZIP)
    assert not img_data == img.image_data


@pytest.mark.skipif(not CompressionType.GZIP.is_supported(), reason="no GZIP support")
def test_uncompress():
    img = MangoImage.from_path("test.jpg")
    img_data = img.image_data
    img.compress(CompressionType.GZIP)
    img.uncompress()
    assert img_data == img.image_data


@pytest.mark.skipif(not EncryptionType.AES128.is_supported(), reason="no AES128 support")
def test_encrypt():
    img = MangoImage.from_path("test.jpg")
    img_data = img.image_data
    img.encrypt(EncryptionType.AES128, "1234567812345678")
    assert not img_data == img.image_data


@pytest.mark.skipif(not EncryptionType.AES128.is_supported(), reason="no AES128 support")
def test_decrypt():
    img = MangoImage.from_path("test.jpg")
    img_data = img.image_data

    enc = img.encrypt(EncryptionType.AES128, "1234567812345678")
    assert enc == True

    dec = img.decrypt("1234567812345678")
    assert dec == True

    assert img_data == img.image_data


def test_save():
    img = MangoImage.from_path("test.jpg")
    img.save("save_test.jpg")


@pytest.mark.skipif(is_root(), reason="can't be tested as root")
def test_save_permission():
    img = MangoImage.from_path("test.jpg")

    with pytest.raises(PermissionError):
        img.save("/test.jpg")


@pytest.mark.skipif(not EncryptionType.AES128.is_supported(), reason="no AES128 support")
def test_iv():
    img = MangoImage.from_path("test.jpg")
    img.encrypt(EncryptionType.AES128, "lol")
    meta = img.meta_data
    iv = meta.iv

    # is a list of random ints
    assert isinstance(iv, list)
    assert len(iv) > 0
    assert isinstance(iv[0], int)
