#pragma once

#define MAX_VEHICLE_INFO_CHAR_BUF 50
#define VIN_LENGTH 17+1

#include <stdint.h>
#include "imgui.h"

enum VehicleClass : int {
	VC_SUV,
	VC_SEDAN,
	VC_PICKUP,
};

static const char* vehicleClassStrings[3] = {
	"SUV",
	"Sedan",
	"Pickup",
};

static const char* vehicleClassStringsLowercase[3] = {
	"suv",
	"sedan",
	"pickup",
};

enum VehicleMake : int {
	VM_ACURA,
	VM_ALFA_ROMEO,
	VM_ASTON_MARTIN,
	VM_AUDI,
	VM_BENTLEY,
	VM_BMW,
	VM_BUICK,
	VM_CADILLAC,
	VM_CHEVROLET,
	VM_CHRYSLER,
	VM_DODGE,
	VM_FERRARI,
	VM_FIAT,
	VM_FORD,
	VM_GENESIS,
	VM_GMC,
	VM_HONDA,
	VM_HYUNDAI,
	VM_INFINITI,
	VM_JAGUAR,
	VM_JEEP,
	VM_KIA,
	VM_LAMBORGHINI,
	VM_LAND_ROVER,
	VM_LEXUS,
	VM_LINCOLN,
	VM_LUCID,
	VM_MASERATI,
	VM_MAZDA,
	VM_MCLAREN,
	VM_MERCEDES_BENZ,
	VM_MINI,
	VM_MITSUBISHI,
	VM_NISSAN,
	VM_POLESTAR,
	VM_PORSCHE,
	VM_RAM,
	VM_RIVIAN,
	VM_SUBARU,
	VM_TESLA,
	VM_TOYOTA,
	VM_VOLKSWAGEN,
	VM_VOLVO,
};

static const char* vehicleMakeStrings[43] = {
	"Acura",
	"Alfa Romeo",
	"Aston Martin",
	"Audi",
	"Bentley",
	"BMW",
	"Buick",
	"Cadillac",
	"Chevrolet",
	"Chrysler",
	"Dodge",
	"Ferrari",
	"Fiat",
	"Ford",
	"Genesis",
	"GMC",
	"Honda",
	"Hyundai",
	"Infiniti",
	"Jaguar",
	"Jeep",
	"Kia",
	"Lamborghini",
	"Land Rover",
	"Lexus",
	"Lincoln",
	"Lucid",
	"Maserati",
	"Mazda",
	"McLaren",
	"Mercedes-Benz",
	"Mini",
	"Mitsubishi",
	"Nissan",
	"Polestar",
	"Porsche",
	"Ram",
	"Rivian",
	"Subaru",
	"Tesla",
	"Toyota",
	"Volkswagen",
	"Volvo"
};

typedef struct _VehicleInfo {
	VehicleClass _class;
	uint16_t year = 2000;
	VehicleMake make;

    // TODO: replace with string
	char model[MAX_VEHICLE_INFO_CHAR_BUF] = "";
	char details[MAX_VEHICLE_INFO_CHAR_BUF] = "";
	char VIN[VIN_LENGTH] = ""; // 17 + null term
    
    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(_class),
                CEREAL_NVP(year),
                CEREAL_NVP(make),
                CEREAL_NVP(model),
                CEREAL_NVP(details),
                CEREAL_NVP(VIN));
    }

}VehicleInfo;

