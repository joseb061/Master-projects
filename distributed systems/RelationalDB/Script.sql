--- Structure 
--- drop table if exist earthers;

drop table if exists climber;
drop table if exists climbing_route;
drop table if exists b_wall;
drop table if exists sport_climbing;
drop table if exists country;
drop table if exists comunity;
drop table if exists spot;
drop table if exists setter;

create table spot(
    spot_id serial,
    spot_name text not null,
    max_grade text null,
    min_grade text null,
    primary key(spot_id)
);

create table setter(
    setter_id serial,
    setter_name text not null,
    bolted_year date null,
    primary key(setter_id)
);


create table comunity(
    comunity_id serial,
    comunity_name text not null,
    spot_id int null references spot(spot_id),
    primary key(comunity_id)
);

create table country(
    country_id serial,
    country_name text not null,
    continent text not null,
    spot_id int null references spot(spot_id),
    comunity_id int null references comunity(comunity_id),
    primary key(country_id)
);


create table b_wall(
    b_wall_id serial,
    bw_route_name text not null,
    max_grade text not null,
    branch text not null, 
    free_climbed text not null, 
    memory text null, 
    country_id int null references country(country_id),
    spot_id int null references spot(spot_id),
    setter_id int null references setter(setter_id),
    primary key(b_wall_id)
);

create table sport_climbing(
    sport_climbing_id serial,
    sc_route_name text not null,
    grade text not null, 
    first_try text null,
    number_tryes decimal,
    onsight_punt decimal,
 	redpoint_punt decimal,
    spot_id int null references spot(spot_id),
    comunity_id int null references comunity(comunity_id),
    primary key (sport_climbing_id)
);


create table climber(
    climber_id serial,
    climber_name text not null,
    surname text not null,
    date_event date not null,
    climber_age int not null,
    sport_climbing_id int null references sport_climbing(sport_climbing_id),
    b_wall_id int null references b_wall(b_wall_id),
    primary key(climber_id)
);



insert into setter (setter_name, bolted_year) values ('Willi Bañales',Null);
insert into setter (setter_name, bolted_year) values ('Bernard Amy and P. Vidailhet',Null);

insert into spot (spot_name, max_grade,min_grade) values ('Mugarra','9a','V+');
insert into spot (spot_name, max_grade,min_grade) values ('Margalef','9b+','IV');
insert into spot (spot_name, max_grade,min_grade) values ('Siurana','9b+','IV');
insert into spot (spot_name, max_grade,min_grade) values ('Montsant','9a','V');
insert into spot (spot_name, max_grade,min_grade) values ('Atxarte','9a','IV');
insert into spot (spot_name, max_grade,min_grade) values ('El chalten','8c','IV');

insert into comunity (comunity_name, spot_id) values ('Basque country',1);
insert into comunity (comunity_name, spot_id) values ('Catalunya',2);

insert into country (country_name, continent, spot_id, comunity_id) values ('Spain','Europe',1,1);
insert into country (country_name, continent, spot_id, comunity_id) values ('Argentina','Latinoamerica',1,1);

insert into b_wall (bw_route_name,max_grade, branch, free_climbed, memory, country_id, spot_id,setter_id) values ('Muga beroa','7a','free climbing','No','Via hecha con tasio, muchisimo viento pero un dia brutal !!',1,1,1);
insert into b_wall (bw_route_name,max_grade, branch, free_climbed, memory, country_id, spot_id,setter_id) values ('culoir Amy','6a/90º','free climbing','yes','viote hecho con Beñ y Victor. Increible pirmera via en patagonia, menuda motivacion!!!!',2,6,2);

insert into sport_climbing (sc_route_name,grade,first_try,number_tryes, onsight_punt, redpoint_punt,comunity_id,spot_id) values ('Malatesta','8b',Null,7.,0.,81.66,1,1);
insert into sport_climbing (sc_route_name,grade,first_try,number_tryes, onsight_punt, redpoint_punt,comunity_id,spot_id) values ('Transilvania','8a',Null,4.,0.,80.66,2,2);
insert into sport_climbing (sc_route_name,grade,first_try,number_tryes, onsight_punt, redpoint_punt,comunity_id,spot_id) values ('Escamarlà','7c+',Null,2.,0.,78.34,2,3);
insert into sport_climbing (sc_route_name,grade,first_try,number_tryes, onsight_punt, redpoint_punt,comunity_id,spot_id) values ('Serra de prades','7c',Null,0.,140.,0.,2,4);


insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Joseba','Hernandez','8/12/2016', 25,1,Null);
insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Joseba','Hernandez','26/02/2019',25,4,Null);
insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Joseba','Hernandez','04/04/21',25,Null,1);
insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Joseba','Hernandez','04/09/2021',25,null,2);

insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Mikel','Aurrekoetxea','13/10/2021',23,2,Null);
insert into climber (climber_name, surname,date_event,climber_age,sport_climbing_id, b_wall_id) values ('Mikel','Aurrekoetxea','04/12/2021',23,3,Null);






