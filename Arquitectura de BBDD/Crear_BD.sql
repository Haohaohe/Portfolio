-- phpMyAdmin SQL Dump
-- version 5.1.0
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 26-05-2021 a las 17:24:35
-- Versión del servidor: 10.4.19-MariaDB
-- Versión de PHP: 7.3.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `discoteca`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `evento`
--

CREATE TABLE `evento` (
  `nombre` varchar(20) NOT NULL,
  `usuarios` varchar(500) NOT NULL,
  `promociones` varchar(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Volcado de datos para la tabla `evento`
--

INSERT INTO `evento` (`nombre`, `usuarios`, `promociones`) VALUES
('Partyhard', '', 'Promocion1');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `mercancia`
--

CREATE TABLE `mercancia` (
  `id` int(11) NOT NULL,
  `nombre` varchar(60) NOT NULL,
  `unidades` int(11) NOT NULL,
  `precio` float NOT NULL,
  `proveedor` varchar(60) NOT NULL,
  `salas` varchar(300) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcado de datos para la tabla `mercancia`
--

INSERT INTO `mercancia` (`id`, `nombre`, `unidades`, `precio`, `proveedor`, `salas`) VALUES
(0, 'Puerto de indias', 10, 18.5, 'carrefour', '1,3,5');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `sala`
--

CREATE TABLE `sala` (
  `id_sala` int(11) NOT NULL,
  `nombre` varchar(60) NOT NULL,
  `estado` int(11) NOT NULL,
  `fecha_disponible` date NOT NULL,
  `usuario` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Volcado de datos para la tabla `sala`
--

INSERT INTO `sala` (`id_sala`, `nombre`, `estado`, `fecha_disponible`, `usuario`) VALUES
(1, 'Tematica', 0, '2021-02-11', 'a'),
(2, 'Cumpleaños', 0, '2021-02-11', 'a'),
(3, 'VIP', 0, '2021-02-11', 'simon'),
(4, 'Electronica', 0, '2021-02-11', 'Luke'),
(71, 'Tematica', 0, '2021-02-11', 'simon'),
(72, 'Tematica', 0, '2021-02-11', 'Luke'),
(73, 'Cumpleaños', 0, '2021-02-12', 'vero');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `usuario`
--

CREATE TABLE `usuario` (
  `dni` varchar(60) NOT NULL,
  `nombre` varchar(60) NOT NULL,
  `password` varchar(60) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcado de datos para la tabla `usuario`
--

INSERT INTO `usuario` (`dni`, `nombre`, `password`) VALUES
('1212121', 'Luke', '3232'),
('12121212', 'a', '3232'),
('123123', 'pedro', 'dkfndf'),
('12345', 'simon', '3232'),
('511511', 'gonzalo', 'gonzalopw');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `venta`
--

CREATE TABLE `venta` (
  `idVenta` varchar(60) NOT NULL,
  `idMerc` varchar(60) NOT NULL,
  `Fecha` varchar(60) NOT NULL,
  `Usuario` varchar(60) NOT NULL,
  `idDiscoteca` varchar(60) NOT NULL,
  `ID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Volcado de datos para la tabla `venta`
--

INSERT INTO `venta` (`idVenta`, `idMerc`, `Fecha`, `Usuario`, `idDiscoteca`, `ID`) VALUES
('00', '000', '000', '000', '000', 1),
('111', '111', '2010-10-12', '33333', '5', 4),
('123', '333', '2010-10-11', ' 656568888', '4', 0),
('33', '33', '33', ' 33', '33', 0),
('787', '787', '787', ' 787', '787', 0);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `mercancia`
--
ALTER TABLE `mercancia`
  ADD PRIMARY KEY (`id`);

--
-- Indices de la tabla `sala`
--
ALTER TABLE `sala`
  ADD PRIMARY KEY (`id_sala`);

--
-- Indices de la tabla `usuario`
--
ALTER TABLE `usuario`
  ADD PRIMARY KEY (`dni`);

--
-- Indices de la tabla `venta`
--
ALTER TABLE `venta`
  ADD PRIMARY KEY (`idVenta`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `sala`
--
ALTER TABLE `sala`
  MODIFY `id_sala` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=74;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
