use std::io::Read;
use base64;
use super::ImageFile;
use meta::Base64ImageMetadata;
use compression;
use compression::CompressionType;

#[derive(Serialize, Deserialize)]
pub struct Base64Image {
    base64: String,
    meta: Base64ImageMetadata,
}

impl Base64Image {
    pub fn new(base64: String, meta: Base64ImageMetadata) -> Base64Image {
        Base64Image { base64, meta }
    }
    pub fn from_file(file_image: &mut ImageFile) -> Base64Image {
        let mut vec = Vec::new();
        let _ = file_image.get_file().read_to_end(&mut vec);
        let muh_base64 = base64::encode(&vec);
        let new_meta = file_image.get_meta();
        Base64Image::new(
            muh_base64.replace("\r\n", ""),
            new_meta.to_base64_metadata(),
        )
    }
    pub fn get_meta(&self) -> Base64ImageMetadata {
        self.meta.clone()
    }
    pub fn get_image(&self) -> String {
        self.base64.clone()
    }

    pub fn compress(&self, comp: CompressionType) -> Base64Image {
        compression::compress(comp, self)
    }

    pub fn uncompress(&self, comp: CompressionType) -> Base64Image {
        compression::uncompress(comp, self)
    }

    pub fn encrypt(&self) -> Base64Image {
        unimplemented!()
    }

    pub fn decrypt(&self, password: String) -> Base64Image {
        unimplemented!()
    }
}
