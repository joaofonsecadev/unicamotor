pub fn get_config(project_name: &str) -> String {
    return r"".replace("CUSTOM_PROJECT_NAME_GOES_HERE", &project_name).to_string();
}