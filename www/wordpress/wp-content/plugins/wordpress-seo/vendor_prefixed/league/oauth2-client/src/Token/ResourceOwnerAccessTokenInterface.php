<?php

/**
 * This file is part of the league/oauth2-client library
 *
 * For the connected copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * @copyright Copyright (c) Alex Bilbie <hello@alexbilbie.com>
 * @license http://opensource.org/licenses/MIT MIT
 * @link http://thephpleague.com/oauth2-client/ Documentation
 * @link https://packagist.org/packages/league/oauth2-client Packagist
 * @link https://github.com/thephpleague/oauth2-client GitHub
 */
namespace YoastSEO_Vendor\League\OAuth2\Client\Token;

interface ResourceOwnerAccessTokenInterface extends \YoastSEO_Vendor\League\OAuth2\Client\Token\AccessTokenInterface
{
    /**
     * Returns the resource owner identifier, if defined.
     *
     * @return string|null
     */
    public function getResourceOwnerId();
}
