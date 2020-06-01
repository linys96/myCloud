-- MySQL dump 10.13  Distrib 8.0.19, for Linux (x86_64)
--
-- Host: localhost    Database: wangpan
-- ------------------------------------------------------
-- Server version	8.0.19

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `file`
--

DROP TABLE IF EXISTS `file`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `file` (
  `code` int NOT NULL AUTO_INCREMENT,
  `precode` int DEFAULT '0',
  `filename` char(128) DEFAULT '0',
  `owner` char(16) DEFAULT NULL,
  `md5sum` char(64) DEFAULT 'directory',
  `filesize` bigint DEFAULT NULL,
  `type` char(1) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `file`
--

LOCK TABLES `file` WRITE;
/*!40000 ALTER TABLE `file` DISABLE KEYS */;
INSERT INTO `file` VALUES (23,0,'lin','lin','directory',4096,'d'),(47,0,'dir1','xiongda','directory',4096,'d'),(48,47,'dir2','xiongda','directory',4096,'d'),(49,48,'dir3','xiongda','directory',4096,'d'),(51,0,'wangdao','wangdao','directory',4096,'d'),(52,0,'hello','wangdao','b1946ac92492d2347c6235b4d2611184',6,'-'),(53,0,'hello','xiongda','b1946ac92492d2347c6235b4d2611184',6,'-'),(54,0,'1.txt','wangdao','b026324c6904b2a9cb4b88d6d61c81d1',2,'-');
/*!40000 ALTER TABLE `file` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `log`
--

DROP TABLE IF EXISTS `log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `log` (
  `username` char(16) DEFAULT NULL,
  `ip` char(32) DEFAULT NULL,
  `optime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `information` char(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `log`
--

LOCK TABLES `log` WRITE;
/*!40000 ALTER TABLE `log` DISABLE KEYS */;
INSERT INTO `log` VALUES ('xiongda','192.168.204.128','2020-04-12 13:35:47','log in'),('xiongda','192.168.204.128','2020-04-12 13:35:48','ls'),('xiongda','192.168.204.128','2020-04-12 13:35:57','cd dir1/dir2/dir3'),('xiongda','192.168.204.128','2020-04-12 13:35:58','ls'),('xiongda','192.168.204.128','2020-04-12 13:35:59','cd'),('xiongda','192.168.204.128','2020-04-12 13:35:59','ls'),('xiongda','192.168.204.128','2020-04-12 13:36:08','gets hello'),('xiongda','192.168.204.128','2020-04-12 13:36:11','help'),('xiongda','192.168.204.128','2020-04-12 13:36:16','exit'),('xiongda','192.168.204.128','2020-04-12 13:36:16','exit'),('wangdao','192.168.204.128','2020-04-12 13:36:37','log in'),('wangdao','192.168.204.128','2020-04-12 13:36:40','ls'),('wangdao','192.168.204.128','2020-04-12 13:36:44','mkdir wangdao'),('wangdao','192.168.204.128','2020-04-12 13:36:45','ls'),('wangdao','192.168.204.128','2020-04-12 13:36:50','puts hello'),('wangdao','192.168.204.128','2020-04-12 13:36:51','ls'),('wangdao','192.168.204.128','2020-04-12 13:37:35','exit'),('xiongda','192.168.204.128','2020-04-12 13:40:41','log in'),('xiongda','192.168.204.128','2020-04-12 13:40:42','ls'),('xiongda','192.168.204.128','2020-04-12 13:40:48','exit'),('xiongda','192.168.204.128','2020-04-12 13:40:48','exit'),('wangdao','192.168.204.128','2020-04-12 13:40:58','log in'),('wangdao','192.168.204.128','2020-04-12 13:41:03','ls'),('wangdao','192.168.204.128','2020-04-12 13:41:10','puts hello;'),('wangdao','192.168.204.128','2020-04-12 13:41:16','put hello'),('wangdao','192.168.204.128','2020-04-12 13:41:21','puts hello'),('wangdao','192.168.204.128','2020-04-12 13:41:26','ls'),('wangdao','192.168.204.128','2020-04-12 13:41:34','exit'),('wangdao','192.168.204.128','2020-04-12 13:41:34','exit'),('xiongda','192.168.204.128','2020-04-12 13:43:48','log in'),('xiongda','192.168.204.128','2020-04-12 13:43:49','ls'),('xiongda','192.168.204.128','2020-04-12 13:43:56','remove hello'),('xiongda','192.168.204.128','2020-04-12 13:44:00','puts hello'),('xiongda','192.168.204.128','2020-04-12 13:44:01','ls'),('wangdao','192.168.204.128','2020-04-12 13:44:49','log in'),('wangdao','192.168.204.128','2020-04-12 13:44:50','ls'),('wangdao','192.168.204.128','2020-04-12 13:44:56','puts 1.txt'),('wangdao','192.168.204.128','2020-04-12 13:44:57','ls'),('xiongda','192.168.204.128','2020-04-12 13:45:10','exit'),('xiongda','192.168.204.128','2020-04-12 13:45:10','exit'),('wangdao','192.168.204.128','2020-04-12 13:45:19','exit'),('wangdao','192.168.204.128','2020-04-12 13:45:19','exit');
/*!40000 ALTER TABLE `log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user` (
  `name` char(16) NOT NULL,
  `salt` char(12) DEFAULT NULL,
  `ciphertext` char(255) DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('lin','$6$IX58upfd','$6$IX58upfd$.gb8nQ3gKWUgtsDj63dpBkCpYAURWUleTDm82HtX.IwKPp19/Nae1vgEdIwly74X0pCXtWyO75w/8nVsFmRZ8/'),('wangdao','$6$TNUPkbjd','$6$TNUPkbjd$cU/VASlBywTsj0oQmG6QQZBNDXU0/EzyXv2x2ZfxKxnIeUKQ91c.GK38DeW5G0ldJF2LxAmxIWi1gMvF8QISE.'),('xiongda','$6$iIKKF5bx','$6$iIKKF5bx$Qc9BejvR7XeM3SneRvxZ.z.1.44PoNSXvsDhuEME.ZetgqPYdmIy23OQ.aEDphrtr.OroZQS/R6we7cVXwj08.'),('xionger','$6$uIC9zfL3','$6$uIC9zfL3$33akfsrRpsrUIP0G2u.qEsimrvXsBGWzl3d41c.6bM6HA9YntXaLaDDVqZ7640HoZaQMfl1cJBi8MDXHKUVGo0');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-04-12 22:06:54
