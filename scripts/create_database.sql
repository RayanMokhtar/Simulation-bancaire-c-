-- Script de création de la base de données pour la simulation bancaire
-- Base de données : simulation_bancaire

USE museevasion_projetcpp;

-- Table pour stocker les configurations de simulation
CREATE TABLE simulation_configurations (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    simulation_duration INT NOT NULL,
    cashier_count INT NOT NULL,
    client_arrival_interval INT NOT NULL,
    min_service_time INT NOT NULL,
    max_service_time INT NOT NULL,
    priority_client_rate DECIMAL(5,4) NOT NULL,
    client_patience_time INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_name (name)
);

-- Table pour stocker les résultats de chaque simulation
CREATE TABLE simulations (
    id INT AUTO_INCREMENT PRIMARY KEY,
    configuration_id INT NOT NULL,
    execution_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    actual_duration INT NOT NULL,
    served_client_count INT NOT NULL,
    non_served_client_count INT NOT NULL,
    average_waiting_time DECIMAL(10,4) NOT NULL,
    average_service_time DECIMAL(10,4) NOT NULL,
    cashier_occupation_rate DECIMAL(5,2) NOT NULL,
    client_satisfaction_rate DECIMAL(5,2) NOT NULL,
    simulation_duration_counter INT NOT NULL,
    cashier_occupation_counter INT NOT NULL,
    FOREIGN KEY (configuration_id) REFERENCES simulation_configurations(id) ON DELETE CASCADE,
    INDEX idx_config_date (configuration_id, execution_date),
    INDEX idx_execution_date (execution_date)
);

-- Table pour stocker les détails des clients traités
CREATE TABLE clients (
    id INT AUTO_INCREMENT PRIMARY KEY,
    simulation_id INT NOT NULL,
    client_type ENUM('ClientNew', 'VIPClient') NOT NULL,
    arrival_time INT NOT NULL,
    service_start_time INT DEFAULT NULL,
    departure_time INT NOT NULL,
    patience_time INT NOT NULL,
    operation_type ENUM('Consultation', 'Transfer', 'Withdraw') NOT NULL,
    operation_service_time INT NOT NULL,
    is_urgent BOOLEAN NOT NULL,
    is_priority BOOLEAN NOT NULL,
    was_served BOOLEAN NOT NULL,
    waiting_time INT DEFAULT NULL,
    actual_service_time INT DEFAULT NULL,
    FOREIGN KEY (simulation_id) REFERENCES simulations(id) ON DELETE CASCADE,
    INDEX idx_simulation_served (simulation_id, was_served),
    INDEX idx_arrival_time (arrival_time),
    INDEX idx_client_type (client_type)
);

-- Table pour stocker l'état des caissiers à chaque instant
CREATE TABLE cashier_states (
    id INT AUTO_INCREMENT PRIMARY KEY,
    simulation_id INT NOT NULL,
    cashier_id INT NOT NULL,
    time_moment INT NOT NULL,
    is_occupied BOOLEAN NOT NULL,
    client_id INT DEFAULT NULL,
    remaining_service_time INT DEFAULT NULL,
    FOREIGN KEY (simulation_id) REFERENCES simulations(id) ON DELETE CASCADE,
    FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE SET NULL,
    INDEX idx_simulation_time (simulation_id, time_moment),
    INDEX idx_cashier_occupied (cashier_id, is_occupied)
);

-- Table pour stocker l'état de la file d'attente à chaque instant
CREATE TABLE queue_states (
    id INT AUTO_INCREMENT PRIMARY KEY,
    simulation_id INT NOT NULL,
    time_moment INT NOT NULL,
    queue_size INT NOT NULL,
    priority_clients_count INT NOT NULL,
    regular_clients_count INT NOT NULL,
    FOREIGN KEY (simulation_id) REFERENCES simulations(id) ON DELETE CASCADE,
    INDEX idx_simulation_time (simulation_id, time_moment)
);

-- Table pour les statistiques agrégées par période
CREATE TABLE period_statistics (
    id INT AUTO_INCREMENT PRIMARY KEY,
    simulation_id INT NOT NULL,
    period_start INT NOT NULL,
    period_end INT NOT NULL,
    clients_arrived INT NOT NULL,
    clients_served INT NOT NULL,
    clients_departed_impatient INT NOT NULL,
    average_queue_size DECIMAL(10,4) NOT NULL,
    average_waiting_time DECIMAL(10,4) NOT NULL,
    cashier_occupation_rate DECIMAL(5,2) NOT NULL,
    FOREIGN KEY (simulation_id) REFERENCES simulations(id) ON DELETE CASCADE,
    INDEX idx_simulation_period (simulation_id, period_start, period_end)
);

-- Vue pour obtenir un résumé des simulations avec leurs configurations
CREATE VIEW simulation_summary AS
SELECT 
    s.id,
    s.execution_date,
    sc.name as configuration_name,
    sc.simulation_duration as configured_duration,
    s.actual_duration,
    sc.cashier_count,
    s.served_client_count,
    s.non_served_client_count,
    s.client_satisfaction_rate,
    s.average_waiting_time,
    s.average_service_time,
    s.cashier_occupation_rate
FROM simulations s
JOIN simulation_configurations sc ON s.configuration_id = sc.id
ORDER BY s.execution_date DESC;

-- Vue pour les statistiques de performance par configuration
CREATE VIEW configuration_performance AS
SELECT 
    sc.name,
    sc.simulation_duration,
    sc.cashier_count,
    COUNT(s.id) as execution_count,
    AVG(s.client_satisfaction_rate) as avg_satisfaction_rate,
    AVG(s.average_waiting_time) as avg_waiting_time,
    AVG(s.cashier_occupation_rate) as avg_occupation_rate,
    MIN(s.execution_date) as first_execution,
    MAX(s.execution_date) as last_execution
FROM simulation_configurations sc
LEFT JOIN simulations s ON sc.id = s.configuration_id
GROUP BY sc.id, sc.name, sc.simulation_duration, sc.cashier_count
ORDER BY avg_satisfaction_rate DESC;

-- Insérer quelques configurations de test
INSERT INTO simulation_configurations (name, simulation_duration, cashier_count, client_arrival_interval, min_service_time, max_service_time, priority_client_rate, client_patience_time) VALUES
('Configuration Standard', 100, 3, 5, 3, 8, 0.2, 15),
('Configuration Haute Charge', 200, 2, 3, 2, 6, 0.3, 10),
('Configuration Service Rapide', 150, 4, 4, 1, 4, 0.25, 20),
('Configuration Test', 50, 2, 2, 2, 5, 0.4, 8);
