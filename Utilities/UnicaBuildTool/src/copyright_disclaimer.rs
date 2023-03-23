use chrono::Datelike;
use tracing::{info, debug};
use crate::GlobalValues;

pub fn write_copyright_disclaimer(global_values: GlobalValues) {
    info!("Applying the copyright disclaimer to source files");

    let current_date = chrono::Utc::now();
    let year = current_date.year();

    let disclaimer = format!("2021-{} Copyright joaofonseca.dev, All Rights Reserved", year);

    debug!("The disclaimer to be written is: '{}'", disclaimer)
}