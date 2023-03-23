use chrono::Datelike;

pub fn write_copyright_disclaimer() {
    let current_date = chrono::Utc::now();
    let year = current_date.year();

    let disclaimer = format!("2021-{} Copyright joaofonseca.dev, All Rights Reserved.", year);
}